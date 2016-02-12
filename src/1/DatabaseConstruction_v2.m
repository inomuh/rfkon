%%%clc;clear all;close all;
refPointCoordinates=xlsread('/home/bolatu/Weka/09.02.2015_Sony/RefPointCoordinates.xlsx');
train_wifi=xlsread('/home/bolatu/Weka/09.02.2015_Sony/Xperia_Train_RemovedNaN_RemovedCisco.xlsx');
% train_wifi=csvread('/home/bolatu/Weka/09.02.2015_Sony/Xperia_Test_RemovedNaN_RemovedCisco.csv');

temp_train_mf=xlsread('/home/bolatu/Weka/09.02.2015_Sony/Xperia_MF_Train.xlsx');

% mf_1 = [0.1; 19.6; -21.3];
% mf_2 = [-18.6; -0.2; -21.2];
% mf_3 = [-18.3; -14.3; -16];
% mf_4 = [-22.9; -13.6; -8.4];
% mf_5 = [-28.8; -0.2; -2.1];
% 
% angle_1 = [0; 0; 0];
% angle_2 = [0; 0; 90];
% angle_3 = [-40; 0; 90];
% angle_4 = [-40; -20; 90];
% angle_5 = [0; -45; 90];
% 
% mf_1 - calibrated_mf(angle_2,mf_2)
% mf_1 - calibrated_mf(angle_3,mf_3)
% mf_1 - calibrated_mf(angle_4,mf_4)
% mf_1 - calibrated_mf(angle_5,mf_5)

train_mf = [];
for i = 1:length(temp_train_mf)
    C = calibrated_mf([0; 0; 85.27], temp_train_mf(i,:)');
    train_mf = [train_mf; C'];
end

trainmean_mf=[];
trainstd_mf=[];

for cn=1:20
    trainmean_mf=[trainmean_mf;mean(train_mf((cn-1)*40+1:cn*40,:))];
    trainstd_mf=[trainstd_mf;std(train_mf((cn-1)*40+1:cn*40,:))];
end

database=zeros(20,44);
database(:,1:2)=[refPointCoordinates(:,2),refPointCoordinates(:,3)];
database(:,39:41)=trainmean_mf;
database(:,42:44)=trainstd_mf;

for cn=1:20
    trainmean_wifi=nanmean(train_wifi((cn-1)*80+1:cn*80,:));
    database(cn,3:20)=trainmean_wifi(:,1:18);
    trainstd_wifi=nanstd(train_wifi((cn-1)*80+1:cn*80,:));
    database(cn,21:38)=trainstd_wifi(:,1:18);
end
%%% header'lar� da ekle sonra
%% col(1):X, col(2):Y, col(3:20):WiFi_mean, col(21:38):WiFi_std, col(39:41):MF_mean, col(42:44):MF_std
xlswrite('database_v2.xlsx',database);