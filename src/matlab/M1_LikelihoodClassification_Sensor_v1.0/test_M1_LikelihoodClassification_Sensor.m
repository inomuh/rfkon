%WiFi,MF,WiFi_MF
clc;clear all;close all;
%% maximum likelihood classification using WiFi data
% M1_LikelihoodClassification_Sensor(1,0,0);

%% maximum likelihood classification using MF data
% M1_LikelihoodClassification_Sensor(0,1,0);

%% maximum likelihood classification using WiFi+WiFi data
M1_LikelihoodClassification_Sensor(0,0,1);
