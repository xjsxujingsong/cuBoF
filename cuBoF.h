#ifndef CUBOF_H
#define CUBOF_H

#include <iostream>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <algorithm> 
#include <stdint.h>

extern "C" {
  #include <vl/kmeans.h>
}

#include "lib/cuSIFT/cudaSift.h"

using namespace std;

class cuBoF {
public:
  // Number of training data
  uint32_t numTrainingImages;

  // Total number of features (or words) in the bag
  uint32_t numFeatures;

  // dimension of feature (128 in the case of SIFT histogram)
  uint32_t numDimensions;

  // These are k-means for clustered SIFT points from training phase
  float *features;

  // Hold inverse document frequency weights
  float *weights;

  // Hold the kdforest
  VlKDForest *kdForest;

  // kdforest options
  uint32_t numTrees;
  uint32_t maxNumComparisons;
  
  // Load pre-computed BoF model from file
  cuBoF(const char *path);

  // Initialize new BoF model
  cuBoF(int numFeatures, int numTrainingImages);
  ~cuBoF();

  // Train BoF model given a array of image file paths
  void train(float *imgData, int w, int h);

  // Quantize and normalize (via IDF) vector of features into visual word
  // vocabulary
  float *vectorize(SiftData *siftData);

  void save(const char *path);

private:
  // Quantize and count features into a feature histogram
  void quantize(SiftData *siftData, float *histogram);

  // Weight feature histogram with IDF
  void weight(float *histogram);

  // Normalize feature histogram to unit length
  void normalize(float *histogram);

  // Create visual word (feature) vocabulary
  void clusterFeatures(int numPts, float *histograms);

  // Build feature vocabulary tree
  void buildFeatureTree();

  // Compute inverse document frequency weights from array of SiftData
  void computeWeights(SiftData *siftData);

  // Extra features on given image
  void extractFeaturesFromImage(float *img, int w, int h, SiftData &siftData);  
};

#endif