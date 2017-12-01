#pragma once
#include "dr_camera.h"

namespace driderSDK {

Camera::Camera() {}

Camera::Camera(const Vector3D& pos,
							 const Vector3D& look,
							 float fov,
							 float nearPlane,
							 float farPlane) {
	m_pos = pos;
	m_up = Vector3D(0.f, 1.f, 0.f);

	//m_projection = ;
	
}

Camera::~Camera() {}

void 
Camera::update(float delta) {

}

void 
Camera::move() {

}

void 
Camera::pan() {

}

void 
Camera::setTarget(const Vector3D& target) {
	m_target = target;
}

void 
Camera::rotate(const Quaternion& rotation) {

}

void 
Camera::orbit() {

}

}