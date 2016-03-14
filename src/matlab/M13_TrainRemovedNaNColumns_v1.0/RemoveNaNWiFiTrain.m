clc;clear all;close all;
[filenameTrainData,pathnameTrainData]=uigetfile({'*.csv'},'Train Data File Selector for Removing NaN Columns','C:\Users\sinem\Documents\GitHub\rfkon\datasets\preprocessed_data');
TrainData_path=strcat(pathnameTrainData,filenameTrainData);
[headerTrainData,valuesTrainData] = csvreadh( TrainData_path);
size(valuesTrainData)
trainmean_wifi=[];
trainstd_wifi=[];
for cn=1:20
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

size(notNaN_WiFiValues)

csvwriteh('C:\Users\sinem\Documents\GitHub\rfkon\datasets\preprocessed_data\01-31.03.2016\09.03.2016_SamsungTablet_WiFi.csv', notNaN_WiFiValues, notNaNMean_WiFiHeaders );
csvwriteh('TrainMeanWiFi_NaNColumns_All.csv', notNaNMean_WiFiValues, notNaNMean_WiFiHeaders );
csvwriteheader('C:\Users\sinem\Documents\GitHub\rfkon\datasets\preprocessed_data\01-31.03.2016\09.03.2016_SamsungTablet_WiFi_Header.csv', notNaNMean_WiFiHeaders);
csvwriteheaderwthRefPoint('TrainWiFi_NaNColumns_Header_withoutRefPoint.csv', notNaNMean_WiFiHeaders);

