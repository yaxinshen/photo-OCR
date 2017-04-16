##content
source code and data to build the dictionary (filter), extract train/test features, and train the linear classifier.

##environment requriement
operating system: Ubuntu 14.04
Libary: OpenCV >= 2.4.9 
        Liblinear >= 1.94(included)
		blas (part of liblinear but maybe potentially replaced by libblas system lib)
		Libsvm 
Sofeware : CMake >=2.8
           C++ compiler

##compile
CMake .
make

##use
0. extract the dataset in ./data/characters

1. Unsupervised learning dictionary(filter) and save them into first_layer_centroids.xml:

./get_dictionary data/characters/icdar/img_ICDAR_train_labels.txt data/characters/chars74k/img_labels.txt data/characters/synthetic/img_labels.txt


2. Extract Features:

for train data:
./extract_features first_layer_centroids.xml data/characters/icdar/img_ICDAR_train_labels.txt > data/all_train_data.svm
./extract_features first_layer_centroids.xml data/characters/chars74k/img_labels.txt >> data/all_train_data.svm
./extract_features first_layer_centroids.xml data/characters/synthetic/img_labels.txt >> data/all_train_data.svm

for test data:
./extract_features first_layer_centroids.xml data/characters/icdar/img_ICDAR_test_labels.txt > data/test_data.svm


3. Scale data:

svm-scale -s data/all_train_data.svm.range data/all_train_data.svm > data/all_train_data.svm.scaled
svm-scale -r data/all_train_data.svm.range data/test_data.svm > data/test_data.svm.scaled2


4. Train the model :

./liblinear-1.94/train -s 2 -c 4 -e 0.001 data/all_train_data.svm.scaled all_train_data.liblinear.model_s2
./liblinear-1.94/train -s 7 -c 4 -e 0.001 data/all_train_data.svm.scaled all_train_data.liblinear.model_s7
it may take a lot time, depending on your memory.


5. Evaluate the test accuracy:

./liblinear-1.94/predict data/test_data.svm.scaled2 all_train_data.liblinear.model_s2 out1
./liblinear-1.94/predict data/test_data.svm.scaled2 all_train_data.liblinear.model_s7 out2


6. the file used in end-to-end system:
   ./first_layer_centroids.xml
   ./data/all_train_data.svm.range
   ./all_train_data.liblinear.model_s2
   ./all_train_data.liblinear.model_s7
   ./liblinear-1.94/linear.h
   ./liblinear-1.94/linear.cpp
   ./liblinear-1.94/tron.h
   ./liblinear-1.94/tron.cpp



 
