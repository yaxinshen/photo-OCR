####Project 
Text Detection and Recognition in Natural Images


##Content
**./OCR0101/.** source code for an end-to-end text detection and recognition system 
                including classifier , model and other resources.

				
##Environment requriement
operating system: Windows 8.1 or higher vision
Libary: OpenCV >= 2.4.9 
        Liblinear >= 1.94(included)
		CLAPACK >= 3.1.1
Sofeware : Visual Studio >=2010

##Usage
Open the project file "./OCR0101.sln" by Visual Studio 2010.
Compile it and run
You need to pass two parameters through the command line : the image and the true ground area of it.
eg.   ./resource/demo.jpg ./resource/ga_demo.txt
The second parameter is used to calculate the precision, recall, and f measure for text detection.
It can be a blank txt file.

