#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

// Function Headers
void detectAndDisplay(Mat frame);

// Global variables
string face_cascade_name = "haarcascade_frontalface_alt.xml";
string eyes_cascade_name = "haarcascade_eye.xml";
string mouth_cascade_name = "haarcascade_mcs_mouth.xml";
string nose_cascade_name = "haarcascade_mcs_nose.xml";

CascadeClassifier eyes_cascade;                                //Utworzenie obiektu cascady oczu 
CascadeClassifier mouth_cascade;                            //Utworzenie obiektu cascady ust 
CascadeClassifier nose_cascade;

CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;

int main(void)
{
	VideoCapture capture(0);

	if (!capture.isOpened())  // check if we succeeded
		return -1;

	// Load the cascade
	if (!face_cascade.load(face_cascade_name))
	{
		printf("--(!)Error loading\n");
		return (-1);
	};

	if (!eyes_cascade.load(eyes_cascade_name))        //Ładowanie pliku ze sprawdzeniem poprawnoci 
	{
		cout << "Nie znaleziono pliku " << eyes_cascade_name << ".";
		return -2;
	}
	if (!mouth_cascade.load(mouth_cascade_name))        //Ładowanie pliku ze sprawdzeniem poprawnoci 
	{
		cout << "Nie znaleziono pliku " << mouth_cascade_name << ".";
		return -2;
	}
	if (!nose_cascade.load(nose_cascade_name))        //Ładowanie pliku ze sprawdzeniem poprawnoci 
	{
		cout << "Nie znaleziono pliku " << nose_cascade_name << ".";
		return -2;
	}

	// Read the video stream
	Mat frame;

	for (;;)
	{
		capture >> frame;

		// Apply the classifier to the frame
		if (!frame.empty())
		{
			detectAndDisplay(frame);
		}
		else
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		int c = waitKey(10);

		if (27 == char(c))
		{
			break;
		}
	}

	return 0;
}

// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	std::vector<Rect> eyes;
	std::vector<Rect> mouth;
	std::vector<Rect> nose;
	Mat frame_gray;
	Mat crop;
	Mat res;
	Mat gray;
	string text;
	stringstream sstm;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// Detect faces
	face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));
	eyes_cascade.detectMultiScale(frame, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
	nose_cascade.detectMultiScale(frame, nose, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));
	mouth_cascade.detectMultiScale(frame, mouth, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(10, 10));

	// Set Region of Interest  // twarz
	cv::Rect roi_b;
	cv::Rect roi_c;

	size_t ic = 0; // ic is index of current element
	int ac = 0; // ac is area of current element

	size_t ib = 0; // ib is index of biggest element
	int ab = 0; // ab is area of biggest element

	// nos
	cv::Rect roi_bn;
	cv::Rect roi_cn;

	size_t icn = 0; // ic is index of current element
	int acn = 0; // ac is area of current element

	size_t ibn = 0; // ib is index of biggest element
	int abn = 0; // ab is area of biggest element

	// oczy
	cv::Rect roi_bo;
	cv::Rect roi_co;

	size_t ico = 0; // ic is index of current element
	int aco = 0; // ac is area of current element

	size_t ibo = 0; // ib is index of biggest element
	int abo = 0; // ab is area of biggest element




	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

	{
		//twarz
		roi_c.x = faces[ic].x;
		roi_c.y = faces[ic].y;
		roi_c.width = (faces[ic].width);
		roi_c.height = (faces[ic].height);


		//twarz
		roi_b.x = faces[ib].x;
		roi_b.y = faces[ib].y;
		roi_b.width = (faces[ib].width);
		roi_b.height = (faces[ib].height);




		crop = frame(roi_b);
		resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
		cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

		// Form a filename
		filename = "";
		stringstream ssfn;
		ssfn << filenumber << ".png";
		filename = ssfn.str();
		filenumber++;

		imwrite(filename, gray);



		for (int i = 0; i < faces.size(); i++)
			rectangle(frame, faces[i], CV_RGB(255, 0, 0), 2);
		for (int i = 0; i < eyes.size(); i++)
			ellipse(frame, Point(eyes[i].x + eyes[i].width / 2, eyes[i].y + eyes[i].height / 2), Size(eyes[i].width / 2, eyes[i].height / 2), 360, 0, 360, CV_RGB(255, 255, 0), 2);
		for (int i = 0; i < nose.size(); i++)
			 rectangle(frame, nose[i], CV_RGB(0, 255, 0), 2);
		for (int i = 0; i < mouth.size(); i++)
			 rectangle(frame, mouth[i], CV_RGB(0, 10, 0), 2);
		

	}

	// Show image
	sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
	text = sstm.str();

	putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
	imshow("original", frame);

	if (!crop.empty())
	{
		imshow("detected", crop);
	}
	else
		destroyWindow("detected");
}
