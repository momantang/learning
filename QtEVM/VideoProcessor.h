#pragma once
#include <QObject>
#include <QDateTime>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "SpatialFilter.h"
#include "string.h"

enum  spatialFilterType
{
	LAPLACIAN,
	GUASSIAN
};
enum temporalFilterType
{
	IIR,
	IDEAL
};
class VideoProcessor:public QObject
{
	Q_OBJECT
	friend class MagnifyDialog;
public:
	explicit VideoProcessor(QObject *parent = 0);
	bool isStop();
	bool isModified();
	bool isOpen();
	bool setDelay(int d);
	long getNumberOfProcessedFrames();
	long getNumberOfPlayedFrames();
	cv::VideoCapture getCaptrue();
	cv::Size getFrameSize();
	long getFrameNumber();
	double getPositionMS();
	double getFrameRate();
	long getLength();
	double getLengthMS();
	int getCodec(char codec[4]);
	void getTempFile(std::string &);
	void getCurTempFile(std::string &);
	void setRelativeRosition(double pos);
	bool setInput(std::string &filename);
	bool setOutput(const std::string& filename, int codec = 0, double framerate = 0.0, bool isColor = true);
	bool setOutput(const std::string& filename, const std::string&ext, int numberofDigits = 3, int startIndex = 0);
	void setSpatialFilter(spatialFilterType type);
	void setTemporalFilter(temporalFilterType type);
	void playIt();
	void pauseIt();
	void stopIt();
	void prevFrame();
	void nextFrame();
	bool jumpTo(long index);
	bool jumpToMS(double pos);
	void close();
	void motionManify();
	void colorManify();
	void writeOutput();
private slots:
	void revertVideo();
signals:
	void showFrame(cv::Mat frame);
	void revert();
	void sleep(int msecs);
	void updateBtn();
	void updateProgressBar();
	void reload(const std::string &);
	void updateProcessProgress(const std::string&message, int percent);
	void closeProgressDialg();
private:


	void calculateLenght();
	bool getNexFrame(Mat &frame);
	bool writeNextFrame(Mat &frame);

	bool createTemp(double framerate = 0.0, bool isColor = true);
	bool spatialFilter(Mat &src, std::vector<Mat> &pyramid);
	void temporalFilter(Mat& src, Mat& dst);
	void temporalIIRFilter(Mat& src, Mat& dst);
	void temporalIdealFilter(Mat& src, Mat& dst);
	void aplify(Mat& src, Mat& dst);
	void attenuate(Mat&src, Mat&dst);
	void concat(const std::vector<Mat> &frames, Mat dst);
	void deConcat(const Mat& src, const cv::Size &frameSize, std::vector<Mat>& frames);
	void createIdealBandPassFilter(Mat &filter, double fl, double fh, double rate);
	cv::VideoCapture capture;
	int delay;
	double rate;
	long fnumber;
	long length;
	bool stop;
	bool modify;
	long curPos;
	int curIndex;
	int curLevel;
	int digits;
	std::string extension;
	spatialFilterType spatialType;
	temporalFilterType temporalType;
	int levels;
	float alpha;
	float lambda_c;
	float fl;
	float fh;
	float chromattenuation;
	float delta;
	float exaggeration_factor;
	float lambda;

	cv::VideoWriter writer;
	cv::VideoWriter tempWriter;

	std::string outputFile;
	std::string tempFile;
	std::vector<std::string> tempFileList;
	std::vector<Mat> lowpass1;
	std::vector<Mat> lowpass2;





	//friend class MagnifyDialog;
};

