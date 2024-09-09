# Compiler
CC = g++

# Project Name
PROJECT = app

DATA = ../data/

DEST = ../bin/

# OpenCV Libraries (pkg-config)
LIBS = `pkg-config --cflags --libs opencv4` 

# OpenCV + OpenGL Libraries (pkg-config)
LIBSALL = `pkg-config --cflags --libs opencv4 gl` 

BINDIR = ../bin

IMDIR = olympus/olympus

FEATURES = features.csv

# Compile and Link
# TASK = featureExtraction
FEATURETYPE = Baseline

# Distance computaation requisites
TASK = distanceFinding
TARGET = pic.0571.jpg
DISTANCETYPE = EuclideanDistance
NUMIMAGES = 5


#	FEATURE EXTRACTOR (without striding)
# app: featureExtraction.cpp featureutils.cpp utils.cpp
# 	$(CC) $^ -o $(BINDIR)/$(TASK) $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/$(TASK) $(DATA)$(IMDIR)/ $(FEATURETYPE) $(DATA)$(FEATURES)

#	FEATURE EXTRACTOR (with striding)
# app: featureExtraction.cpp featureutils.cpp utils.cpp
# 	$(CC) $^ -o $(BINDIR)/$(TASK) $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/$(TASK) $(DATA)$(IMDIR)/ $(FEATURETYPE) $(DATA)$(FEATURES)

#	DISTANCE FINDER
# app: distanceFinding.cpp distanceutils.cpp utils.cpp
# 	$(CC) $^ -std=c++17 -o $(BINDIR)/$(TASK) $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/$(TASK) $(DATA)$(FEATURES) $(DISTANCETYPE) $(FEATURETYPE) $(DATA)$(IMDIR)/$(TARGET) $(NUMIMAGES)

# # to run the whole pipeline
# app: playground.cpp utils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# # # to run the whole pipeline
# app: detectTargetPNP.cpp utils.cpp calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -O3 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground


# # to run the whole pipeline
# app: cameraCalibration.cpp utils.cpp calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -O3 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# to run the whole pipeline
# app: videoPlayground.cpp utils.cpp calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: temp.cpp utils.cpp calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: playground3d.cpp utils.cpp customGraphics.cpp#calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -g -Werror -Wall -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: temp.cpp utils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -O3 -g -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: detectTargetProjectPoints.cpp utils.cpp calibutils.cpp customGraphics.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -O3 -g -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground


app: workingExtension.cpp utils.cpp calibutils.cpp customGraphics.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
	$(CC) $^ -std=c++17 -O3 -g -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
	$(BINDIR)/playground


# app: objectDisplay.cpp utils.cpp calibutils.cpp customGraphics.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -O3 -g -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: harrisCorners.cpp utils.cpp calibutils.cpp#featureutils.cpp# distanceutils.cpp featureutils.cpp utils.cpp hog.cpp
# 	$(CC) $^ -std=c++17 -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

	
# app: playground2.cpp featureutils.cpp distanceutils.cpp  utils.cpp db_utils.cpp classification.cpp segmentation.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: vidToIms.cpp utils.cpp featureutils.cpp classification.cpp segmentation.cpp db_utils.cpp distanceutils.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: imToDb.cpp utils.cpp featureutils.cpp classification.cpp segmentation.cpp db_utils.cpp distanceutils.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: inference.cpp utils.cpp featureutils.cpp classification.cpp segmentation.cpp db_utils.cpp distanceutils.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground


# app: scratch.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

# app: main.cpp featureutils.cpp distanceutils.cpp  utils.cpp db_utils.cpp classification.cpp segmentation.cpp
# 	$(CC) $^ -o $(BINDIR)/playground $(LIBS) -I../include -DENABLE_PRECOMPILED_HEADERS=OFF
# 	$(BINDIR)/playground

