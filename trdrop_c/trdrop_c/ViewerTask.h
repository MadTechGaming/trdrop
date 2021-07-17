#pragma once
#ifndef TRDROP_TASKS_POST_VIEWER_H
#define TRDROP_TASKS_POST_VIEWER_H

#include <functional>
#include <math.h>
#include <sstream>
#include <iomanip>

#include <C:\dev\opencv2\core\core.hpp>

#include "Tasks.h"
#include "utilvideo.h"

namespace trdrop {
	namespace tasks {
		namespace post {

			using trdrop::tasks::posttask;

			class ViewerTask : public posttask {

				// default member
			public:
				ViewerTask() = delete;
				ViewerTask(const ViewerTask & other) = default;
				ViewerTask & operator=(const ViewerTask & other) = delete;
				ViewerTask(ViewerTask && other) = default;
				ViewerTask & operator=(ViewerTask && other) = delete;
				~ViewerTask() = default;

				// specialized member
			public:
				ViewerTask(int delay, cv::Size size)
					: delay(delay)
					, size(size)
					, posttask(std::bind(&ViewerTask::process
						, this
						, std::placeholders::_1
						, std::placeholders::_2))
				{}

				ViewerTask(cv::Size size)
					: ViewerTask(1, size) {}

				// interface methods
			public:
				void process(cv::Mat & res, const size_t currentFrameIndex) {
					trdrop::util::video::showFrame(res, delay);
				}

				// public member
			public:
				std::function<void()> init = [&]() { trdrop::util::video::initVideoFrame(); trdrop::util::video::resize(size); };

				// private member
			private:
				const int delay;
				const cv::Size size;
			};
		} // namespace post
	} // namespace tasks
} // namespace trdrop

#endif // !TDROP_TASKS_POST_VIEWER_H
