#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>

#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "linear.h"


#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <direct.h>  
#include <io.h>

#include"TextDetection.h"

using namespace cv;
using namespace std;

struct model* model_;
Mat filters, M, P;

char predict_char(string test_img)
{
  int image_size  = 32;
  int quad_size   = 12;
  int patch_size  = 8;
  int num_quads   = 25; // extract 25 quads (12x12) from each image
  int num_tiles   = 25; // extract 25 patches (8x8) from each quad 

  double alpha    = 0.5; // used for feature representation: 
                         // scalar non-linear function z = max(0, |D*a| - alpha)

  Mat quad;
  Mat tmp;

  int patch_count = 0;
  vector< vector<double> > data_pool(9); 

  Mat img  = imread(test_img);
  /*if(img.channels() != 3)
  {
    cout << "ERROR: image must be RGB" << endl;
    exit(-1);
  }*/
  cvtColor(img,img,COLOR_RGB2GRAY);

  resize(img,img,Size(image_size,image_size));

  int quad_id = 1;
  for (int q_x=0; q_x<=image_size-quad_size; q_x=q_x+(quad_size/2-1))
  {
    for (int q_y=0; q_y<=image_size-quad_size; q_y=q_y+(quad_size/2-1))
    {
      Rect quad_rect = Rect(q_x,q_y,quad_size,quad_size); 
      img(quad_rect).copyTo(quad);

      //start sliding window (8x8) in each tile and store the patch as row in data_pool
      for (int w_x=0; w_x<=quad_size-patch_size; w_x++)
      {
        for (int w_y=0; w_y<=quad_size-patch_size; w_y++)
        {
          quad(Rect(w_x,w_y,patch_size,patch_size)).copyTo(tmp);
          tmp = tmp.reshape(0,1);
          tmp.convertTo(tmp, CV_64F);
          normalizeAndZCA(tmp,M,P);
          vector<double> patch;
          tmp.copyTo(patch);
          if ((quad_id == 1)||(quad_id == 2)||(quad_id == 6)||(quad_id == 7))
            data_pool[0].insert(data_pool[0].end(),patch.begin(),patch.end());
          if ((quad_id == 2)||(quad_id == 7)||(quad_id == 3)||(quad_id == 8)||(quad_id == 4)||(quad_id == 9))
            data_pool[1].insert(data_pool[1].end(),patch.begin(),patch.end());
          if ((quad_id == 4)||(quad_id == 9)||(quad_id == 5)||(quad_id == 10))
            data_pool[2].insert(data_pool[2].end(),patch.begin(),patch.end());
          if ((quad_id == 6)||(quad_id == 11)||(quad_id == 16)||(quad_id == 7)||(quad_id == 12)||(quad_id == 17))
            data_pool[3].insert(data_pool[3].end(),patch.begin(),patch.end());
          if ((quad_id == 7)||(quad_id == 12)||(quad_id == 17)||(quad_id == 8)||(quad_id == 13)||(quad_id == 18)||(quad_id == 9)||(quad_id == 14)||(quad_id == 19))
            data_pool[4].insert(data_pool[4].end(),patch.begin(),patch.end());
          if ((quad_id == 9)||(quad_id == 14)||(quad_id == 19)||(quad_id == 10)||(quad_id == 15)||(quad_id == 20))
            data_pool[5].insert(data_pool[5].end(),patch.begin(),patch.end());
          if ((quad_id == 16)||(quad_id == 21)||(quad_id == 17)||(quad_id == 22))
            data_pool[6].insert(data_pool[6].end(),patch.begin(),patch.end());
          if ((quad_id == 17)||(quad_id == 22)||(quad_id == 18)||(quad_id == 23)||(quad_id == 19)||(quad_id == 24))
            data_pool[7].insert(data_pool[7].end(),patch.begin(),patch.end());
          if ((quad_id == 19)||(quad_id == 24)||(quad_id == 20)||(quad_id == 25))
            data_pool[8].insert(data_pool[8].end(),patch.begin(),patch.end());
          patch_count++;
        }
      }

      quad_id++;
    }
  }

  //do dot product of each normalized and whitened patch 
  //each pool is averaged and this yields a representation of 9xD 
  Mat feature = Mat::zeros(9,filters.rows,CV_64FC1);
  for (int i=0; i<9; i++)
  {
    Mat pool = Mat(data_pool[i]);
    pool = pool.reshape(0,data_pool[i].size()/filters.cols);
    for (int p=0; p<pool.rows; p++)
    {
      for (int f=0; f<filters.rows; f++)
      {
        feature.row(i).at<double>(0,f) = feature.row(i).at<double>(0,f) + max(0.0,std::abs(pool.row(p).dot(filters.row(f)))-alpha);
      }
    }
  }
  feature = feature.reshape(0,1);


  // data must be normalized within the range obtained during training
  double lower = -1.0;
  double upper =  1.0;
  Mat feature_min = Mat::zeros(1,feature.cols,CV_64FC1);
  Mat feature_max = Mat::ones(1,feature.cols,CV_64FC1);
  std::ifstream range_infile("./resource/all_train_data.svm.range");
  std::string line;
  //discard first two lines
  getline(range_infile, line);
  getline(range_infile, line);
  while (getline(range_infile, line))
  {
    istringstream iss(line);
    int idx;
    double min_val, max_val;
    if (!(iss >> idx >> min_val >> max_val)) 
    {
       cout << "ERROR: reading svm-scale ranges file " << endl; 
       exit(0); 
    } 
    feature_min.at<double>(0,idx-1) = min_val;
    feature_max.at<double>(0,idx-1) = max_val;
  }


  for (int k=0; k<feature.cols; k++)
  {
    if(feature.at<double>(0,k) == feature_min.at<double>(0,k))
      feature.at<double>(0,k) = lower;
    else if(feature.at<double>(0,k) == feature_max.at<double>(0,k))
      feature.at<double>(0,k) = upper;
    else   
      feature.at<double>(0,k) = lower + (upper-lower) *
                                (feature.at<double>(0,k)-feature_min.at<double>(0,k))/
                                (feature_max.at<double>(0,k)-feature_min.at<double>(0,k));
  }

  struct feature_node *x = (struct feature_node *) 
                           malloc((feature.cols+1)*sizeof(struct feature_node));

  for (int k=0; k<feature.cols; k++)
  {
     x[k].index = k+1; // liblinear labels start at 1 not 0
     x[k].value = feature.at<double>(0,k);
  }
  x[feature.cols].index = -1;

  double predict_label = predict(model_,x);
  string ascii = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyx0123456789";

  char res = ascii[predict_label-1];
  //free_and_destroy_model(&model_);
  free(x);
  return res;
}

vector<string> getFiles(string cate_dir)  
{  
    vector<string> files;//存放文件名  
  	char base[100];
    _finddata_t file;  
    long lf;  
    //输入文件夹路径  
    if ((lf=_findfirst(cate_dir.c_str(), &file)) == -1) {  
        cout<<cate_dir<<" not found!!!"<<endl;  
    } else {  
        while(_findnext(lf, &file) == 0) {  
            //输出文件名  
            //cout<<file.name<<endl;  
            if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)  
                continue; 
			sprintf(base,"./out/%s",file.name) ;
            files.push_back(base);  
        }  
    }  
    _findclose(lf);  

  
    //排序，按从小到大排序  
    sort(files.begin(), files.end());  
    return files;  
}  


int main(int argc, char **argv)
{
	model_=load_model("./resource/all_train_data.liblinear.model_s7");
	FileStorage fs("./resource/first_layer_centroids.xml", FileStorage::READ);
    fs["D"] >> filters;
    fs["M"] >> M;
    fs["P"] >> P;
    fs.release();

	vector<string> gtset;string line;
	cv::Mat image = cv::imread(argv[1]);
	ifstream fin(argv[2]);
	//cv::Mat image = cv::imread("./resource/demo.jpg");
	//ifstream fin("./resource/gt.txt");
	while(getline(fin,line))
	{
		gtset.push_back(line);
	}
        fin.clear();  fin.close(); 

	TextDetection td;
	cout<<td.detectText(image,gtset)<<endl;

	int candidateNum = td.canNum;
	cout <<"the number of output candidate:"<<candidateNum<<endl;
	//cv::waitKey();

   	string filePath = "./out//*";
	vector<string> files=getFiles(filePath);
     
vector<char> res; 

for (int i = 0;i < candidateNum;i++)  
{   
  string file = files[i];
  char temp = predict_char(file);
  res.push_back(temp);
    //res[i]=p.first;
}

free_and_destroy_model(&model_);

string aline;
ifstream px("./paixu.txt");

vector<char> output;
int total=0, pos = 0;

while(getline(px,aline))
{
  istringstream iss(aline);
  pos = total;
  int ii;
  while(iss>>ii)
  {
	  int tpos = ii+pos;
	  //if (tpos>res.size())break;
	  output.push_back(res[tpos]);
	  total++;
  }
  output.push_back('\n');
  //if (total == res.size())break;
}

/*vector<char> output;
int total=0, pos = 0;

for(int ii=0;ii<paixu.size();ii++)
{
    pos=total;
    string temp = paixu[ii];
    int jj = 0;
	for(jj=0;jj<temp.size();jj++)
	{
       char num = temp[jj];
		if(num>='0'&&num<='9')
       {
			int tpos= num-'0'+pos;
			output.push_back(res[tpos]);
			total++;
       }
    }
    output.push_back('\n');     
}	   */

for(int k =0;k<output.size();k++)
cout<<output[k];

cout<<endl;	  

cv::waitKey();
}
