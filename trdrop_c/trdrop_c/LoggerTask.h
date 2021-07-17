#pragma once
#ifndef TRDROP_TASKS_INTER_LOGGER_H
#define TRDROP_TASKS_INTER_LOGGER_H

#include <functional>
#include <math.h>
#include <sstream>
#include <iomanip>

#include <C:\dev\opencv2\core\core.hpp>

#include "Tasks.h"
#include "utilvideo.h"

namespace trdrop {
	namespace tasks {
		namespace inter {

			using trdrop::tasks::intertask;

			template <class Formatter>
			class LoggerTask : public intertask {

			// types
			public:
				using tostring = std::function<std::string(size_t)>;

				// default member
			public:
				LoggerTask() = delete;
				LoggerTask(const LoggerTask & other) = default;
				LoggerTask & operator=(const LoggerTask & other) = default;
				LoggerTask(LoggerTask && other) = default;
				LoggerTask & operator=(LoggerTask && other) = default;
				//~LoggerTask() = default;

				// specialized member
			public:
				LoggerTask(std::vector<tostring> & conversions
					, std::vector<std::string> ids
					, std::vector<std::string> fileNames
					, std::string logName)
					: conversions(conversions)
					, ids(ids)
					, fileNames(fileNames)
					, logName(logName)
					, formatter(Formatter())
					, intertask(std::bind(&LoggerTask::process
						, this
						, std::placeholders::_1
						, std::placeholders::_2
						, std::placeholders::_3))
				{

					std::for_each(fileNames.begin(), fileNames.end(), [&](std::string filename) {

						streams.emplace_back(std::shared_ptr<std::ofstream>(new std::ofstream{ toLogFileName(filename) }));
#if _TR_DEBUG
						std::cout << "DEBUG: LoggerTask - created stream \"" << toLogFileName(filename) << "\"\n";
#endif				
						std::string formatted(formatter.format(ids.begin(), ids.end()));
						streams.back() -> write(formatted.c_str(), formatted.size());
					});

				}

				~LoggerTask()
				{
					std::for_each(streams.begin(), streams.end(), [&](std::shared_ptr<std::ofstream> & streamptr) {
						streamptr -> close();
#if _TR_DEBUG
						std::cout << "DEBUG: LoggerTask - deleted stream\n";
#endif
					});
				}

				// interface methods
			public:
				void process(cv::Mat & res, const size_t currentFrameIndex, const size_t vix) {

					static std::mutex mutex;
					std::lock_guard<std::mutex> lock(mutex);
	
					std::vector<std::string> values;
					std::transform(conversions.begin(), conversions.end(), std::back_inserter(values), [&](tostring f) {
							return f(vix);
					});

					std::string formatted(formatter.format(values.begin(), values.end()));
#if _TR_DEBUG
					std::cout << "DEBUG: LoggerTask - vix: " << vix << '\n';
#endif
					streams[vix] -> write(formatted.c_str(), formatted.size());
					values.clear();
				}

				// private member
			private:

				std::function<std::string(std::string)> toLogFileName = [&](std::string filename) {
					size_t dotIndex = filename.find_last_of('.');
					filename = dotIndex == -1 ? filename : filename.substr(0, dotIndex);

					dotIndex = logName.find_last_of('.');
 					std::string preLogName = dotIndex == -1 ? logName : logName.substr(0, dotIndex);
					std::string extention = dotIndex == -1 ? "" : logName.substr(dotIndex, logName.size());

					size_t slashIndex = filename.find_last_of('\\');
					filename = slashIndex == -1 ? filename : filename.substr(slashIndex + 1);

					return preLogName + "-" + filename + extention;
				};

				Formatter formatter;
				std::vector<std::shared_ptr<std::ofstream>> streams;
				std::vector<tostring> & conversions;
				std::vector<std::string> ids;
				std::vector<std::string> fileNames;
				std::string logName;
			};	
		} // namespace inter
	} // namespace tasks
} // namespace trdrop

#endif // !TDROP_TASKS_INTER_LOGGER
