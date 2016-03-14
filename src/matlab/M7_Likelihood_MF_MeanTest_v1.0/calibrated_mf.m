function [C]  = calibrated_mf( angle, mf)
%ROTATIONMATRIX3D Summary of this function goes here
%   Detailed explanation goes here

% pitch, roll, yaw are in degrees
% pitch is with respect to x axis
% roll is with respect to y axis
% yaw is with respect to z axis
% Rotation matrix 3d = rotz * rotx * roty 
% Calibrated mf vector = rotation matrix 3d * mf vector 
% roll angle must be taken negative 

angle_x = -angle(1);
angle_y = angle(2);
angle_z = -angle(3);

mf_x = mf(1);
mf_y = mf(2);
mf_z = mf(3);

%     R = zeros(3,3);
%     R(1,1) = cosd(angle_z) * cosd(angle_y) - sind(angle_z) * sind(angle_x) * sind(angle_y);
%     R(1,2) = -sind(angle_z) * cosd(angle_x);
%     R(1,3) =  cosd(angle_z) * sind(angle_y) + sind(angle_z) * sind(angle_x) * cosd(angle_y);
%     R(2,1) =  sind(angle_z) * cosd(angle_y) + cosd(angle_z) * sind(angle_x) * sind(angle_y);
%     R(2,2) = cosd(angle_z) * cosd(angle_x);
%     R(2,3) = sind(angle_z) * sind(angle_y) - cosd(angle_z) * sind(angle_x) * cosd(angle_y);
%     R(3,1) = -cosd(angle_x) * sind(angle_y);
%     R(3,2) = sind(angle_x);
%     R(3,3) = cosd(angle_x) * cosd(angle_y);
%   C = R * [mf_x; mf_y; mf_z]; 


  C = rotz(angle_z) * rotx(angle_x) * roty(angle_y) * [mf_x; mf_y; mf_z]; 


end

