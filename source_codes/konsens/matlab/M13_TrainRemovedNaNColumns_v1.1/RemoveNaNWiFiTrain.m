clc;clear all;close all;
[filenameTrainData,pathnameTrainData]=uigetfile({'*.csv'},'Train Data File Selector for Removing NaN Columns','./datasets');
TrainData_path=strcat(pathnameTrainData,filenameTrainData);
[headerTrainData,valuesTrainData] = csvreadh( TrainData_path);

trainmean_wifi=[];
trainstd_wifi=[];
for cn=1:20
    std=nanstd(valuesTrainData((cn-1)*80+1:cn*80,:));
    if std==0
        disp('Zero standart deviation')
        continue;
    end
    trainmean_wifi=[trainmean_wifi;nanmean(valuesTrainData((cn-1)*80+1:cn*80,:))];
    trainstd_wifi=[trainstd_wifi;nanstd(valuesTrainData((cn-1)*80+1:cn*80,:))];
end

indices = find(isnan(trainmean_wifi) == 1);
[nanRows,nanColumns] = ind2sub(size(trainmean_wifi),indices);

nanColumns_WiFi=unique(nanColumns);

%% All wifi train data with not NaN cloumns
notNaN_WiFiValues=valuesTrainData;
notNaN_WiFiValues(:,nanColumns_WiFi)=[];

%% Mean wifi train data with not NaN cloumns
notNaNMean_WiFiValues=trainmean_wifi;
notNaNMean_WiFiValues(:,nanColumns_WiFi)=[];

%% All wifi train data headers with not NaN cloumns
notNaNMean_WiFiHeaders=headerTrainData;
notNaNMean_WiFiHeaders(:,nanColumns_WiFi)=[];

csvwriteh('./datasets/WiFi_Train_wthNaNMeanColumns.csv', notNaN_WiFiValues, notNaNMean_WiFiHeaders );

[filenameTestData,pathnameTestData]=uigetfile({'*.csv'},'Test Data File Selector for Ordering Columns according to Train WiFi data','./datasets');
TestData_path=strcat(pathnameTestData,filenameTestData);
[headerTestData,valuesTestData] = csvreadh( TestData_path);

testWiFiData=[];
headerNewTestData=[];
for i=1:size(notNaNMean_WiFiHeaders,2)
    for j=1:size(headerTestData,2)
        if isequal(notNaNMean_WiFiHeaders(1,i),headerTestData(1,j))
            headerNewTestData=[headerNewTestData,headerTestData(1,j)];
            testWiFiData=[testWiFiData,valuesTestData(:,j)];
        end
    end
end

csvwriteh('./datasets/WiFi_Test_wthNaNMeanColumns.csv', testWiFiData, headerNewTestData );
