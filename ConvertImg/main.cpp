//
// пример работы с ROI
//

#include <cv.h>
#include <highgui.h>x
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <dirent.h>
#include <vector>
#include <string>

IplImage* image = 0;

int CountFilesInFolder(char* path){
	int file_count = 0;
	int dir_count = 0;
	int total = 0;

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path); 
	while ((entry = readdir(dirp)) != NULL) {
		if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			total++;
	}
	dir_count = total - file_count;
	closedir(dirp);
	return dir_count;
}

void AviToPng(std::string name,double countFrames, int num){

	// TODO
	IplImage* frame =0;
	int counter = 0;
	// имя файла задаётся первым параметром
	char filename[512];
	char filename1[512];
	sprintf(filename1, "Res/Test%d", num);
	_mkdir(filename1);

	// получаем информацию о видео-файле
	char tab2[1024];
	strncpy(tab2, name.c_str(), sizeof(tab2));
	tab2[sizeof(tab2) - 1] = 0;
	sprintf(filename, "Input/%s", tab2);
	CvCapture* capture = cvCreateFileCapture( filename );
	double maxFrame = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	int fps = (int)(maxFrame/countFrames), tempFps = 0;
	while(1){
		// получаем следующий кадр
		frame = cvQueryFrame( capture ); 
		if( !frame ) {
			break;
		}
		if( tempFps == fps){
			sprintf(filename1, "Res/Test%d/Image%d.jpg", num, counter);
			cvSaveImage(filename1, frame);
			counter++;
			tempFps = 0;
		}
		tempFps++;
		char c = cvWaitKey(1);
		if (c == 27) { // если нажата ESC - выходим
			break;
		}
	}
	// освобождаем ресурсыvector
	cvReleaseCapture( &capture );
}

void CutImages(int num){
	// задаём ROI
	int x = 283;
	int y = 58;
	int width = 560;
	int height = 524;
	char filename [512];

	sprintf(filename, "Res/Test%d/", num);
	int countFiles = CountFilesInFolder(filename);

	for (int counter = 0; counter < countFiles; counter++){
		// имя картинки задаётся первым параметром
		sprintf(filename, "Res/Test%d/Image%d.jpg", num, counter);

		// получаем картинку
		image = cvLoadImage(filename,1);

		// устанавливаем ROI
		cvSetImageROI(image, cvRect(x,y,width,height));

		// показываем изображение
		//cvShowImage("ROI", image);

		// сохраняем изображение
		cvSaveImage(filename, image);
		cvResetImageROI(image);

		// освобождаем ресурсы
		cvReleaseImage( &image );
		cvDestroyAllWindows(); 
	}
}

// Я не победила char*
std::vector<std::string> DirvectorName(char* path){
	int file_count = 0;
	int dir_count = 0;
	int total = 0;
	std::vector<std::string> tempvector;
	char* arr[100];
	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path); 
	while ((entry = readdir(dirp)) != NULL) {
		if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
			std::string name = entry->d_name;
			printf("***\n%s\n",name);
			tempvector.push_back(name);
			dir_count++;
		}
	}
	closedir(dirp);
	return tempvector;
}

char* DirvectorName1(char* path){
	int file_count = 0;
	int dir_count = 0;
	int total = 0;

	DIR * dirp;
	struct dirent * entry;

	dirp = opendir(path); 
	while ((entry = readdir(dirp)) != NULL) {
		if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
			char* name = new char[500];
			name = entry->d_name;
			return name;
		}
	}
	dir_count = total - file_count;
	closedir(dirp);
	return "None";
}

void Start(){
	std::vector<std::string> vector = DirvectorName("Input/");
	char * name = DirvectorName1("Input/");
	for (int counter = 0;((counter < vector.size()) || (counter < CountFilesInFolder("Input/"))); counter++){
		AviToPng(vector.at(counter), 1000.0, counter);
		CutImages(counter);
	}
}
int main(int argc, char* argv[])
{
	Start();
	return 0;
}