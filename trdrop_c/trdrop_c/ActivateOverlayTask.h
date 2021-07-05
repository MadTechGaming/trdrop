#pragma once
#ifndef TRDROP_TASKS_POST_ACTIVATE_OVERLAY_H
#define TRDROP_TASKS_POST_ACTIVATE_OVERLAY_H

#include <functional>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <C:\dev\opencv2\core\core.hpp>

#include "Tasks.h"
#include "framealgorithm.h"

namespace trdrop {
	namespace tasks {
		namespace post {

			using trdrop::tasks::posttask;

			class ActivateOverlayTask : public posttask {

				// default member
			public:
				ActivateOverlayTask(const ActivateOverlayTask & other) = default;
				ActivateOverlayTask & operator=(const ActivateOverlayTask & other) = delete;
				ActivateOverlayTask(ActivateOverlayTask && other) = default;
				ActivateOverlayTask & operator=(ActivateOverlayTask && other) = delete;
				~ActivateOverlayTask() = default;

				// specialized member
			public:
				ActivateOverlayTask()
					: posttask(std::bind(&ActivateOverlayTask::process
						, this
						, std::placeholders::_1
						, std::placeholders::_2))
				{
#if _TR_DEBUG
					std::cout << "ActivateOverlayTask was initialized\n";
#endif
				}

				// interface methods
			public:
				void process(cv::Mat & res, const size_t currentFrameIndex) {
					trdrop::algorithm::make_transparent(res);
				}

				// private methods
			private:

				// private member
			private:

			};
		} // namespace post
	} // namespace tasks
} // namespace trdrop

#endif // !TRDROP_TASKS_POST_ACTIVATE_OVERLAY_H
