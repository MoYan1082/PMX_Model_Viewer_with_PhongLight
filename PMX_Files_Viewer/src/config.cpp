#include "config.h"

Config::Config(std::string path) {
	this->file.open(path, cv::FileStorage::READ);
	if (!this->file.isOpened()) {
		std::cerr << "file " << path << " open failed!" << std::endl;
		exit(-1);
	}
}
Config::~Config() {
	if (this->file.isOpened())
		this->file.release();
}
