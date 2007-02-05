// Copyright (C) 2002-2006 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __S_VIEW_FRUSTUM_H_INCLUDED__
#define __S_VIEW_FRUSTUM_H_INCLUDED__

#include "plane3d.h"
#include "vector3d.h"
#include "aabbox3d.h"
#include "matrix4.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{

	//! Defines the view frustum. Thats the space viewed by the camera.
	/** The view frustum is enclosed by 6 planes. These six planes share
	four points. A bounding box around these four points is also stored in
	this structure.
	*/
	struct SViewFrustum
	{
		enum VFPLANES
		{
			//! Far plane of the frustum. Thats the plane farest away from the eye.
			VF_FAR_PLANE = 0,
			//! Near plane of the frustum. Thats the plane nearest to the eye.
			VF_NEAR_PLANE,
			//! Left plane of the frustum.
			VF_LEFT_PLANE,
			//! Right plane of the frustum.
			VF_RIGHT_PLANE,
			//! Bottom plane of the frustum.
			VF_BOTTOM_PLANE,
			//! Top plane of the frustum.
			VF_TOP_PLANE,

			//! Amount of planes enclosing the view frustum. Should be 6.
			VF_PLANE_COUNT
		};

		//! Default Constructor
		SViewFrustum() {};

		//! This constructor creates a view frustum based on a projection and/or
		//! view matrix.
		SViewFrustum(const core::matrix4& mat);

		//! This constructor creates a view frustum based on a projection
		//! and/or view matrix.
		void setFrom(const core::matrix4& mat);

		//! the position of the camera
		core::vector3df cameraPosition;

		//! all planes enclosing the view frustum.
		core::plane3d<f32> planes[VF_PLANE_COUNT];


		//! transforms the frustum by the matrix
		//! \param mat: Matrix by which the view frustum is transformed.
		void transform(const core::matrix4 &mat);

		//! returns the point which is on the far left upper corner inside the
		//! the view frustum.
		core::vector3df getFarLeftUp() const;

		//! returns the point which is on the far left bottom corner inside the
		//! the view frustum.
		core::vector3df getFarLeftDown() const;

		//! returns the point which is on the far right top corner inside the
		//! the view frustum.
		core::vector3df getFarRightUp() const;

		//! returns the point which is on the far right bottom corner inside the
		//! the view frustum.
		core::vector3df getFarRightDown() const;

		//! returns a bounding box enclosing the whole view frustum
		const core::aabbox3d<f32> &getBoundingBox() const;

		//! recalculates the bounding box member based on the planes
		inline void recalculateBoundingBox();

		//! bouding box around the view frustum
		core::aabbox3d<f32> boundingBox;

		//! Hold a copy of important transform matrices
		enum E_TRANSFORMATION_STATE_3
		{
			ETS_VIEW_PROJECTION_3 = video::ETS_PROJECTION + 1,
			ETS_VIEW_MODEL_INVERSE_3,
			ETS_CURRENT_3,
			ETS_COUNT_3
		};

		core::matrix4 Matrices[ETS_COUNT_3];
		void setTransformState( video::E_TRANSFORMATION_STATE state);
	};


	//! transforms the frustum by the matrix
	//! \param Matrix by which the view frustum is transformed.
	inline void SViewFrustum::transform(const core::matrix4 &mat)
	{
		for (int i=0; i<VF_PLANE_COUNT; ++i)
			mat.transformPlane(planes[i]);

		mat.transformVect(cameraPosition);
		recalculateBoundingBox();
	}


	//! returns the point which is on the far left upper corner inside the
	//! the view frustum.
	inline core::vector3df SViewFrustum::getFarLeftUp() const
	{
		core::vector3df p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far left bottom corner inside the
	//! the view frustum.
	inline core::vector3df SViewFrustum::getFarLeftDown() const
	{
		core::vector3df p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_LEFT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far right top corner inside the
	//! the view frustum.
	inline core::vector3df SViewFrustum::getFarRightUp() const
	{
		core::vector3df p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_TOP_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}

	//! returns the point which is on the far right bottom corner inside the
	//! the view frustum.
	inline core::vector3df SViewFrustum::getFarRightDown() const
	{
		core::vector3df p;
		planes[scene::SViewFrustum::VF_FAR_PLANE].getIntersectionWithPlanes(
			planes[scene::SViewFrustum::VF_BOTTOM_PLANE],
			planes[scene::SViewFrustum::VF_RIGHT_PLANE], p);

		return p;
	}

	//! returns a bounding box enclosing the whole view frustum
	inline const core::aabbox3d<f32> &SViewFrustum::getBoundingBox() const
	{
		return boundingBox;
	}

	//! recalculates the bounding box member based on the planes
	inline void SViewFrustum::recalculateBoundingBox()
	{
		boundingBox.reset ( cameraPosition );

		boundingBox.addInternalPoint(getFarLeftUp());
		boundingBox.addInternalPoint(getFarRightUp());
		boundingBox.addInternalPoint(getFarLeftDown());
		boundingBox.addInternalPoint(getFarRightDown());
	}

	//! This constructor creates a view frustum based on a projection
	//! and/or view matrix.
	inline SViewFrustum::SViewFrustum(const core::matrix4& mat)
	{
		setFrom ( mat );
	}

/*
	//! This constructor creates a view frustum based on a projection
	//! and/or view matrix.
	inline void SViewFrustum::setFrom(const core::matrix4& mat)
	{
		// left clipping plane
		planes[SViewFrustum::VF_LEFT_PLANE].Normal.X = -(mat(0,3) + mat(0,0));
		planes[SViewFrustum::VF_LEFT_PLANE].Normal.Y = -(mat(1,3) + mat(1,0));
		planes[SViewFrustum::VF_LEFT_PLANE].Normal.Z = -(mat(2,3) + mat(2,0));
		planes[SViewFrustum::VF_LEFT_PLANE].D =		  -(mat(3,3) + mat(3,0));

		// right clipping plane
		planes[SViewFrustum::VF_RIGHT_PLANE].Normal.X = -(mat(0,3) - mat(0,0));
		planes[SViewFrustum::VF_RIGHT_PLANE].Normal.Y = -(mat(1,3) - mat(1,0));
		planes[SViewFrustum::VF_RIGHT_PLANE].Normal.Z = -(mat(2,3) - mat(2,0));
		planes[SViewFrustum::VF_RIGHT_PLANE].D =        -(mat(3,3) - mat(3,0));

		// top clipping plane
		planes[SViewFrustum::VF_TOP_PLANE].Normal.X = -(mat(0,3) - mat(0,1));
		planes[SViewFrustum::VF_TOP_PLANE].Normal.Y = -(mat(1,3) - mat(1,1));
		planes[SViewFrustum::VF_TOP_PLANE].Normal.Z = -(mat(2,3) - mat(2,1));
		planes[SViewFrustum::VF_TOP_PLANE].D =        -(mat(3,3) - mat(3,1));

		// bottom clipping plane
		planes[SViewFrustum::VF_BOTTOM_PLANE].Normal.X = -(mat(0,3) + mat(0,1));
		planes[SViewFrustum::VF_BOTTOM_PLANE].Normal.Y = -(mat(1,3) + mat(1,1));
		planes[SViewFrustum::VF_BOTTOM_PLANE].Normal.Z = -(mat(2,3) + mat(2,1));
		planes[SViewFrustum::VF_BOTTOM_PLANE].D =        -(mat(3,3) + mat(3,1));

		// near clipping plane
		planes[SViewFrustum::VF_NEAR_PLANE].Normal.X = -mat(0,2);
		planes[SViewFrustum::VF_NEAR_PLANE].Normal.Y = -mat(1,2);
		planes[SViewFrustum::VF_NEAR_PLANE].Normal.Z = -mat(2,2);
		planes[SViewFrustum::VF_NEAR_PLANE].D =        -mat(3,2);

		// far clipping plane
		planes[SViewFrustum::VF_FAR_PLANE].Normal.X = -(mat(0,3) - mat(0,2));
		planes[SViewFrustum::VF_FAR_PLANE].Normal.Y = -(mat(1,3) - mat(1,2));
		planes[SViewFrustum::VF_FAR_PLANE].Normal.Z = -(mat(2,3) - mat(2,2));
		planes[SViewFrustum::VF_FAR_PLANE].D =        -(mat(3,3) - mat(3,2));
		// normalize normals

		for (s32 i=0; i<6; ++i)
		{
			const f32 len = core::reciprocal_squareroot ( planes[i].Normal.getLengthSQ() );
			planes[i].Normal *= len;
			planes[i].D *= len;
		}

		// make bounding box
		recalculateBoundingBox();
	}
*/

	//! This constructor creates a view frustum based on a projection
	//! and/or view matrix.
	inline void SViewFrustum::setFrom(const core::matrix4& mat)
	{
		const f32 * m = mat.M;

		// left clipping plane
		planes[VF_LEFT_PLANE].Normal.X	= m[3 ] + m[0];
		planes[VF_LEFT_PLANE].Normal.Y	= m[7 ] + m[4];
		planes[VF_LEFT_PLANE].Normal.Z	= m[11] + m[8];
		planes[VF_LEFT_PLANE].D			= m[15] + m[12];

		// right clipping plane
		planes[VF_RIGHT_PLANE].Normal.X = m[3 ] - m[0];
		planes[VF_RIGHT_PLANE].Normal.Y = m[7 ] - m[4];
		planes[VF_RIGHT_PLANE].Normal.Z = m[11] - m[8];
		planes[VF_RIGHT_PLANE].D =        m[15] - m[12];

		// top clipping plane
		planes[VF_TOP_PLANE].Normal.X = m[3 ] - m[1];
		planes[VF_TOP_PLANE].Normal.Y = m[7 ] - m[5];
		planes[VF_TOP_PLANE].Normal.Z = m[11] - m[9];
		planes[VF_TOP_PLANE].D =        m[15] - m[13];

		// bottom clipping plane
		planes[VF_BOTTOM_PLANE].Normal.X = m[3 ] + m[1];
		planes[VF_BOTTOM_PLANE].Normal.Y = m[7 ] + m[5];
		planes[VF_BOTTOM_PLANE].Normal.Z = m[11] + m[9];
		planes[VF_BOTTOM_PLANE].D =        m[15] + m[13];

		// far clipping plane
		planes[VF_FAR_PLANE].Normal.X = m[3 ] - m[2];
		planes[VF_FAR_PLANE].Normal.Y = m[7 ] - m[6];
		planes[VF_FAR_PLANE].Normal.Z = m[11] - m[10];
		planes[VF_FAR_PLANE].D =        m[15] - m[14];

		// near clipping plane
		planes[VF_NEAR_PLANE].Normal.X = m[2];
		planes[VF_NEAR_PLANE].Normal.Y = m[6];
		planes[VF_NEAR_PLANE].Normal.Z = m[10];
		planes[VF_NEAR_PLANE].D =        m[14];

		
		// normalize normals
		u32 i;
		for ( i=0; i != 6; ++i)
		{
			const f32 len = - core::reciprocal_squareroot ( planes[i].Normal.getLengthSQ() );
			planes[i].Normal *= len;
			planes[i].D *= len;
		}

		// make bounding box
		recalculateBoundingBox();
	}

	inline void SViewFrustum::setTransformState( video::E_TRANSFORMATION_STATE state)
	{
		switch ( state )
		{
			case video::ETS_VIEW:
				Matrices[ETS_VIEW_PROJECTION_3].setbyproduct_nocheck (	Matrices[ video::ETS_PROJECTION],
																		Matrices[ video::ETS_VIEW]
																	);
				Matrices[ETS_VIEW_MODEL_INVERSE_3] = Matrices[ video::ETS_VIEW];
				Matrices[ETS_VIEW_MODEL_INVERSE_3].makeInverse();
				break;

			case video::ETS_WORLD:
				Matrices[ETS_CURRENT_3].setbyproduct (  Matrices[ ETS_VIEW_PROJECTION_3 ],
														Matrices[ video::ETS_WORLD]	);
				break;

		}
	}


} // end namespace scene
} // end namespace irr

#endif

