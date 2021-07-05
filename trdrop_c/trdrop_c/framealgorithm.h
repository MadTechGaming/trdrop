#pragma once
#ifndef TRDROP_FRAMEALGORITHM_H
#define TRDROP_FRAMEALGORITHM_H

#include <iostream>

#include<C:\dev\opencv2/core/core.hpp>

#include "util.h"

namespace trdrop {
	namespace algorithm {

		/*
		* Supports following types if the dim is fitting
		* http://docs.opencv.org/trunk/dc/dc2/mat_8hpp.html#Typedefs
		*/
		template <class T>
		bool are_equal(const cv::Mat & frameA, const cv::Mat & frameB) {
			return std::equal(frameA.begin<T>(), frameA.end<T>(), frameB.begin<T>());
		}

		/*
		 * Specialisation for short circuit compare
		 */
		template <>
		bool are_equal<uchar*>(const cv::Mat & frameA, const cv::Mat & frameB) {
			int rindex;
			for (int i = 0; i < frameA.rows; ++i) {
				rindex = i * frameA.rows;
				for (int j = 0; j < frameA.cols; ++j) {
					if (frameA.data[rindex + j] != frameB.data[rindex + j]) return false;
				}
			}
			return true;
		}

		/*
		* Specialisation for compare in greyscale + only every 7 pixels and a pixelDifference
		* 
		* TODO test for performance with skipping pixels & Greyscale
		*/
		template <typename T>
		bool are_equal_with(const cv::Mat & frameA, const cv::Mat & frameB, int pixelDifference) {
			cv::Mat bw_a;
			cv::Mat bw_b;
			cv::cvtColor(frameA, bw_a, CV_BGR2GRAY);
			cv::cvtColor(frameB, bw_b, CV_BGR2GRAY);

			for (int i = 0; i < bw_a.rows; ++i) {
				for (int j = 0; j < bw_a.cols; j += 7) {
					int ac(std::max(bw_a.at<uchar>(i, j), bw_b.at<uchar>(i, j)));
					int bc(std::min(bw_a.at<uchar>(i, j), bw_b.at<uchar>(i, j)));
					if (ac - bc > pixelDifference) {
						return false;
					}
				}
			}
			return true;
		}

		/*
		 * Set every pixel visibilty to 0 (%), we need to delete all BGR data too, because of how alpha copying works in opencv (see PlotTask.drawGrid)
		 * Invariant: We need to have at least an image encoding of at least 4 bytes per pixel, e.g CV_8UC4
		 */
		void make_transparent(cv::Mat & src) {
			for (int i = 0; i < src.rows - 10; ++i) {
				for (int j = 0; j < src.cols; ++j) {
					auto & pixel = src.at<cv::Vec4b>(i, j);
					pixel[0] = 0;
					pixel[1] = 0;
					pixel[2] = 0;
					pixel[3] = 0;
				}
			}
		}


	} // namespace algorithm
} // namespace trdrop

#endif // !TRDROP_FRAMEALGORITHM_H
