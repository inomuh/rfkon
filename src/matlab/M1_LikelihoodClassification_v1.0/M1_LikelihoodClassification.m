%% Clear everything
clc;clear all;close all;

%% Load Train Database
%% Load ref. point coordinates
% filenameRefPointCoordinates: Ref. Point Coordinates data file name
% pathnameRefPointCoordinates: Ref. Point Coordinates data path name
[filenameRefPointCoordinates,pathnameRefPointCoordinates]=uigetfile({'*.xls;*.xlsx'},'Ref. Point Coordinates File Selector');
% testTmp_wifi_path: a file path name of Ref. Point Coordinates data for xlsread function
refPointCoordinates_path=strcat(pathnameRefPointCoordinates,filenameRefPointCoordinates);
refPointCoordinates=xlsread(refPointCoordinates_path);

% noRefPoints: number of Ref. Points
noRefPoints=size(refPointCoordinates,1);

%% Load train wifi data 
% filenameTrainWiFi: train wifi data file name
% pathnameTrainWiFi: train wifi data path name
[filenameTrainWiFi,pathnameTrainWiFi]=uigetfile({'*.xls;*.xlsx'},'WiFi Train File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\WiFi');
% trainTmp_wifi_path: a file path name of wifi train data for xlsread function
trainTmp_wifi_path=strcat(pathnameTrainWiFi,filenameTrainWiFi);
trainTmp_wifi=xlsread(trainTmp_wifi_path,'WiFi');

%% Load train mf data
% filenameTrainMF: train mf data file name
% pathnameTrainMF: train mf data path name
[filenameTrainMF,pathnameTrainMF]=uigetfile({'*.xls;*.xlsx'},'MF Train File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\MF');
% trainTmp_mf_path: a file path name of mf train data for xlsread function
trainTmp_mf_path=strcat(pathnameTrainMF,filenameTrainMF);
temp_train_mf=xlsread(trainTmp_mf_path,'MF');

%% Construct train database that contain ref. point x, y coordinates, train WiFi mean values, 
% train WiFi st. dev. values, train MF mean values, and train MF st. dev.
% values
DatabaseConstruction(refPointCoordinates,noRefPoints,trainTmp_wifi,temp_train_mf)
%% Load constructed database for train data that contains ref. points coordinates,
% mean and st. dev. of train WiFi RSS data, and mean and st. dev. of train MF data
% filenameTrainDatabase: constructed train database file name
% pathnameTrainDatabase: constructed train database path name
[filenameTrainDatabase,pathnameTrainDatabase]=uigetfile({'*.xls;*.xlsx'},'Database File Selector');
% trainDatabase_path: a file path name of constructed train database for xlsread function
trainDatabase_path=strcat(pathnameTrainDatabase,filenameTrainDatabase);
database=xlsread(trainDatabase_path);

 %% Load test wifi data
% filenameTest: test wifi data file name
% pathnameTest: test wifi data path name
[filenameTest,pathnameTest]=uigetfile({'*.xls;*.xlsx'},'WiFi Test File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\WiFi');
% testTmp_wifi_path: a file path name of wifi test data for xlsread function
testTmp_wifi_path=strcat(pathnameTest,filenameTest);
testTmp_wifi=xlsread(testTmp_wifi_path,'WiFi');

%% Load test mf data
% filenameTestMF: test mf data file name
% pathnameTestMF: test mf data path name
[filenameTestMF,pathnameTestMF]=uigetfile({'*.xls;*.xlsx'},'MF Test File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\MF');
% testTmp_mf_path: a file path name of mf train data for xlsread function
testTmp_mf_path=strcat(pathnameTestMF,filenameTestMF);
testTmp_mf=xlsread(testTmp_mf_path,'MF');

accuracy=CalculateAccuracy(testTmp_wifi,testTmp_mf,database)

