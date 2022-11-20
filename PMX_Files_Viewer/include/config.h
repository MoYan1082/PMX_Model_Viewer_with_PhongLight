#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

class Config {
private:
	cv::FileStorage file;

public:
	Config(std::string path);
	~Config();


	template<typename T>
	void get(const char* key, T& value) {
		this->file[key] >> value;
	}
};
#endif // CONFIG_H