// Copyright 2014 Isis Innovation Limited and the authors of InfiniTAM

#pragma once

#include <map>

#include "../../Engines/Visualisation/Interface/ITMVisualisationEngine.h"
#include "../../Objects/RenderStates/ITMRenderStateFactory.h"
#include "../../Objects/Scene/ITMScene.h"
#include "../../Objects/Tracking/ITMTrackingState.h"
#include "../../Utils/ITMLibSettings.h"

namespace ITMLib {
	struct ITMPoseConstraint
	{
	public:
		ITMPoseConstraint(void);

		void AddObservation(const ORUtils::SE3Pose & relative_pose, int weight = 1);
		ORUtils::SE3Pose GetAccumulatedObservations(void) const { return accu_poses; }
		int GetNumAccumulatedObservations(void) const { return accu_num; }

	private:
		ORUtils::SE3Pose accu_poses;
		int accu_num;
	};

	typedef std::map<int,ITMPoseConstraint> ConstraintList;

	template<class TVoxel,class TIndex>
	class ITMLocalScene
	{
	public:
		ITMScene<TVoxel,TIndex> *scene;
		ITMRenderState *renderState;
		ITMTrackingState *trackingState;
		ConstraintList relations;
		ORUtils::SE3Pose estimatedGlobalPose;

		ITMLocalScene(const ITMLibSettings *settings, const Vector2i & trackedImageSize)
		{
			MemoryDeviceType memoryType = settings->deviceType == ITMLibSettings::DEVICE_CUDA ? MEMORYDEVICE_CUDA : MEMORYDEVICE_CPU;
			scene = new ITMScene<TVoxel,TIndex>(&settings->sceneParams, settings->swappingMode == ITMLibSettings::SWAPPINGMODE_ENABLED, memoryType);
			renderState = ITMRenderStateFactory<TIndex>::CreateRenderState(trackedImageSize, scene->sceneParams, memoryType);
			trackingState = new ITMTrackingState(trackedImageSize, memoryType);
		}
		~ITMLocalScene(void)
		{
			delete scene;
			delete renderState;
			delete trackingState;
		}
	};
}
