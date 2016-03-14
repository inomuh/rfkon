function [testDataSizeIncrementer]=Likelihood(hObject, eventdata, handles)

%% Test Data sayaci
testDataSizeIncrementer=0;
testDataSize=0;
results=[];
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
[filenameTrainWiFi,pathnameTrainWiFi]=uigetfile({'*.csv'},'WiFi Train File Selector','D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.1');
% trainTmp_wifi_path: a file path name of wifi train data for xlsread function
trainTmp_wifi_path=strcat(pathnameTrainWiFi,filenameTrainWiFi);
trainTmp_wifi=csvread(trainTmp_wifi_path,1,0);

numberofAPs=size(trainTmp_wifi,2)-1;


% filenameTrainMF: train mf data file name
% pathnameTrainMF: train mf data path name
[filenameTrainMF,pathnameTrainMF]=uigetfile({'*.csv'},'MF Train File Selector','D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.1');
% trainTmp_mf_path: a file path name of mf train data for xlsread function
trainTmp_mf_path=strcat(pathnameTrainMF,filenameTrainMF);
temp_train_mf_database=csvread(trainTmp_mf_path,1,0);
temp_train_mf=temp_train_mf_database(:,1:3);

%% Construct train database that contain ref. point x, y coordinates, train WiFi mean values, 
% train WiFi st. dev. values, train MF mean values, and train MF st. dev.
% values
DatabaseConstruction(refPointCoordinates,noRefPoints,trainTmp_wifi,temp_train_mf)
%% Load constructed database for train data that contains ref. points coordinates,
% mean and st. dev. of train WiFi RSS data, and mean and st. dev. of train MF data
% filenameTrainDatabase: constructed train database file name
% pathnameTrainDatabase: constructed train database path name
[filenameTrainDatabase,pathnameTrainDatabase]=uigetfile({'*.csv'},'Database File Selector');
% trainDatabase_path: a file path name of constructed train database for xlsread function
trainDatabase_path=strcat(pathnameTrainDatabase,filenameTrainDatabase);
database=csvread(trainDatabase_path,0,0);

data=xlsread('C:\Users\sinem\Desktop\masaüstü\Dataset_Construction_PlotPoints\RefPointCoordinates');
filename='Classification_Results.txt';
header = {'Time', 'Actual','Predicted'};


if exist(filename)==0
    %% ClassificationResults.txt oluþmadýysa
    outid = fopen (filename, 'w+');
    [hrow hcol] = size (header);
    % write header
    for idx = 1:hcol
        fprintf (outid, '%s', header{idx});
        if idx ~= hcol
            fprintf (outid, ',');
        else
            fprintf (outid, '\n' );
        end
    end
else
    %% ClassificationResults.txt oluþtuysa
    outid = fopen (filename, 'a+');
end

while 1
    %% Load test wifi data
    % filenameTest: test wifi data file name
    % pathnameTest: test wifi data path name
%     [filenameTest,pathnameTest]=uigetfile({'*.csv'},'WiFi Test File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\WiFi');
%     % testTmp_wifi_path: a file path name of wifi test data for xlsread function
%     testTmp_wifi_path=strcat(pathnameTest,filenameTest);
    testTmp_wifiAll=csvread('D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.1\wifi.csv',1,0);
    sizetestwifi=size(testTmp_wifiAll,1);
    %% wifi.csv test datasýný içeren dosyadan en sondaki elemaný classification için alýr
    testTmp_wifi=testTmp_wifiAll(size(testTmp_wifiAll,1),:);
    %% Load test mf data
    % filenameTestMF: test mf data file name
    % pathnameTestMF: test mf data path name
%     [filenameTestMF,pathnameTestMF]=uigetfile({'*.csv'},'MF Test File Selector','D:\RFKON_UB_SB\src\M1_LikelihoodClassification_v1.0\datasets\MF');
%     % testTmp_mf_path: a file path name of mf train data for xlsread function
%     testTmp_mf_path=strcat(pathnameTestMF,filenameTestMF);
    testTmp_mf=csvread('D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.1\mf.csv',1,0);
    %% mf.csv test datasýný içeren dosyadan en sondaki elemaný classification için alýr
   
    %%%%%%%%%%%%%%%%%%%%%% BURASI 7:9 o.þ. deðiþecek %%%%%%%%%%%%%%%%%%%%%% 
    test_mf=testTmp_mf(size(testTmp_mf,1),1:3);
    
    [predicted,actual]=CalculateAccuracy_Plot_Refresh(testTmp_wifi,test_mf,database,numberofAPs);
      
    Points=[data(predicted,1),data(predicted,2),data(predicted,3)];
    
    h1=plot(Points(1,2), Points(1,3), 'ro');
    t1=text(Points(1,2), Points(1,3), num2str(Points(1,1)), 'VerticalAlignment','bottom', ...
                         'HorizontalAlignment','left');
    
    pause(1);
    delete(h1);
    delete(t1);
    if isequal(testDataSizeIncrementer,0)
%     if size(testTmp_wifiAll,1)== 1 && testDataSizeIncrementer==0
        testDataSizeIncrementer=testDataSizeIncrementer+1;
        testDataSize=size(testTmp_wifiAll,1);
        new_data = {datestr(now,'yyyy/mm/dd HH:MM PM'), actual, predicted}; 
        fprintf(outid,'%s',datestr(now,'yyyy/mm/dd HH:MM PM'));
        fprintf(outid,',');
        fprintf(outid,'%d',actual);
        fprintf(outid,',');
        fprintf(outid,'%d',predicted);
        fprintf(outid,'\n');
        results=[results;new_data];
    else if testDataSize~=size(testTmp_wifiAll,1)
        testDataSizeIncrementer=testDataSizeIncrementer+1;
        testDataSize=size(testTmp_wifiAll,1);
        new_data = {datestr(now,'yyyy/mm/dd HH:MM PM'), actual, predicted}; 
        fprintf(outid,'%s',datestr(now,'yyyy/mm/dd HH:MM PM'));
        fprintf(outid,',');
        fprintf(outid,'%d',actual);
        fprintf(outid,',');
        fprintf(outid,'%d',predicted);
        fprintf(outid,'\n');
        results=[results;new_data];
    end
    set(handles.text3, 'String', num2str(testDataSizeIncrementer)) ;
    handles.output = hObject;
    guidata(hObject, handles);
end

end
fclose(outid);

 
   

