clc;
clear all;
close all;
%% Load ref. point coordinates
% filenameRefPointCoordinates: Ref. Point Coordinates data file name
% pathnameRefPointCoordinates: Ref. Point Coordinates data path name
[filenameRefPointCoordinates,pathnameRefPointCoordinates]=uigetfile({'*.csv'},'Ref. Point Coordinates File Selector');
% testTmp_wifi_path: a file path name of Ref. Point Coordinates data for xlsread function
refPointCoordinates_path=strcat(pathnameRefPointCoordinates,filenameRefPointCoordinates);
refPointCoordinates=csvread(refPointCoordinates_path,1,0);

% noRefPoints: number of Ref. Points
noRefPoints=size(refPointCoordinates,1);

%% Load train wifi data 
% filenameTrainWiFi: train wifi data file name
% pathnameTrainWiFi: train wifi data path name
[filenameTrainWiFi,pathnameTrainWiFi]=uigetfile({'*.csv'},'WiFi Train File Selector','D:\RFKON_UB_SB\src\M2_Likelihood_WiFi_AllTest_v1.0\datasets\WiFi');
% trainTmp_wifi_path: a file path name of wifi train data for xlsread function
trainTmp_wifi_path=strcat(pathnameTrainWiFi,filenameTrainWiFi);
trainTmp_wifi=csvread(trainTmp_wifi_path,1,0);


%% Construct train database that contain ref. point x, y coordinates, train WiFi mean values, 
% train WiFi st. dev. values, train MF mean values, and train MF st. dev.
% values
DatabaseConstruction(refPointCoordinates,noRefPoints,trainTmp_wifi)
%% Load constructed database for train data that contains ref. points coordinates,
% mean and st. dev. of train WiFi RSS data, and mean and st. dev. of train MF data
% filenameTrainDatabase: constructed train database file name
% pathnameTrainDatabase: constructed train database path name
[filenameTrainDatabase,pathnameTrainDatabase]=uigetfile({'*.csv'},'Database File Selector');
% trainDatabase_path: a file path name of constructed train database for xlsread function
trainDatabase_path=strcat(pathnameTrainDatabase,filenameTrainDatabase);
database=csvread(trainDatabase_path,0,0);

 %% Load test wifi data
% filenameTest: test wifi data file name
% pathnameTest: test wifi data path name
[filenameTest,pathnameTest]=uigetfile({'*.csv'},'WiFi Test File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_Sensor_v1.0\datasets\WiFi');
% testTmp_wifi_path: a file path name of wifi test data for xlsread function
testTmp_wifi_path=strcat(pathnameTest,filenameTest);
testTmp_wifi=csvread(testTmp_wifi_path,1,0);


%%
CalculateAccuracy_Sensor(testTmp_wifi,database,trainTmp_wifi);

