function [predicted_index_using_mf,class_wifi]=CalculateAccuracy_Plot_Refresh(testTmp_wifi,testTmp_mf,database,numberofAPs)
    
    refPointNumber=size(database,1);

    test_wifi=testTmp_wifi;
    
    % mf data is calibrated according to orientation of the mobile device
    test_mf = [];
%     for i = 1:length(testTmp_mf)
%         C = calibrated_mf([0; 0; 85.27], testTmp_mf(i,:)');
%         test_mf = [test_mf; C'];
%     end
%% calibrated mf test data
%     C = calibrated_mf([0; 0; 85.27], testTmp_mf(1,:)');
%     test_mf = [test_mf; C'];
    
%% not calibrated mf test data
    test_mf=testTmp_mf;
    
    %%
    [rowTestWiFiData columnTestWiFiData]=size(test_wifi);
    predictedArray=zeros(rowTestWiFiData,2);
    %nm: likelihood fonksiyonunu kullanarak her bir test datanın tüm mean'i
    %hesaplanmış train dataların birbirleriyle olan olasılıklarını hesaplıyor
    nm_wifi=zeros(size(test_wifi,1),refPointNumber);
    nm_mf=zeros(size(test_wifi,1),refPointNumber);
    %confusion_matrix: weka'daki confusion matrix
    confusion_matrix=zeros(refPointNumber,refPointNumber);

    %BB: her bir test datanın büyükten küçüğe sıralanmış halini tutar
    %II: sıralı indeksleri tutar
    index=1;
    [BB,II] = sort(test_wifi(index,1:columnTestWiFiData-1)','ascend');
    B=BB';
    I=II';
    tmp=B;
    %testmean_sorted: sıralanmış elemanlardan nan olmayanları alır
    %testmean_sorted_index: testmean_sorted indexlerini tutuyor
    test_sorted_wifi=B(~isnan(B));
    test_sorted_index_wifi=I(:,1:length(test_sorted_wifi));

    trainmean_wifi=[];
    trainstd_wifi=[];
    for i=1:length(test_sorted_wifi)
        trainmean_wifi=[trainmean_wifi,database(:,test_sorted_index_wifi(i)+2)];
        trainstd_wifi=[trainstd_wifi,database(:,test_sorted_index_wifi(i)+(2+numberofAPs))];
    end


    for cn2=1:refPointNumber
        k1=1./(trainstd_wifi(cn2,:)*((2*pi)^0.5));
        k2=(test_sorted_wifi-trainmean_wifi(cn2,:)).^2;
        k3=2*(trainstd_wifi(cn2,:).^2);
        k4=k1.*exp(-1*(k2./k3));
        nm_wifi(index,cn2)=prod(k4);
    end

    class_wifi=test_wifi(index,columnTestWiFiData);
    maxIndex_wifi=find(nm_wifi(index,:)==max(nm_wifi(index,:)));

    %%%%%%%% Manyetik Alan ile konumlandırma doğruluğunu iyileştirme  %%%%%%%%

    predicted_refpoint_using_wifi=maxIndex_wifi
    
    %% WiFi ile invalid test data gelmesi durumunu çözme
    while length(predicted_refpoint_using_wifi)>1
        disp('Invalid WiFi test data');
        return;
    end

    trainmean_mf=[];
    trainstd_mf=[];

    from=size(database,2)-(2*size(test_mf,2)-1);
    to=size(database,2)-size(test_mf,2);
    trainmean_mf=database(:,from:to);
    trainstd_mf=database(:,(to+1):size(database,2));
    
    %% komşuluk hesaplama
    %%%komşulukları database'den uzaklıkları 6'dan küçük olanları alacak
    %%%  sqrt((x2-x1)^2+(y2-y1)^2)).
    predicted_x=database(predicted_refpoint_using_wifi,1);
    predicted_y=database(predicted_refpoint_using_wifi,2);
    komsulukArray=find(sqrt((predicted_x-database(:,1)).^2+(predicted_y-database(:,2)).^2)<6);
    %%%temp_komsuluk_wo_zeros_mf = komsuluk(predicted_refpoint_using_wifi,1:max(find(komsuluk(predicted_refpoint_using_wifi,:) > 0)));
    temp_komsuluk_wo_zeros_mf=komsulukArray';
    sub_traindata_mf = [];
    sub_testdata_mf =  test_mf(index,:);
    sub_trainmean_mf=[];
    sub_trainstd_mf=[];
    for cnj = 1:length(temp_komsuluk_wo_zeros_mf)
        sub_trainmean_mf=[sub_trainmean_mf;trainmean_mf(temp_komsuluk_wo_zeros_mf(1,cnj),:)];
        sub_trainstd_mf=[sub_trainstd_mf;trainstd_mf(temp_komsuluk_wo_zeros_mf(1,cnj),:)];
    end

    %% MF komşuluk datası ile test MF datayı likelihood algoritması ile sınıflandırma

    for cn1=1:length(temp_komsuluk_wo_zeros_mf)        
        k1=1./(sub_trainstd_mf(cn1,:)*((2*pi)^0.5));
        k2=(sub_trainmean_mf(cn1,:)-test_mf(index,:)).^2;
        k3=2*(sub_trainstd_mf(cn1,:).^2);
        k4=k1.*exp(-1*(k2./k3));
        nm_mf(index,temp_komsuluk_wo_zeros_mf(1,cn1))=prod(k4);
    end


    predicted_index_using_mf=find(nm_mf(index,:)== max(nm_mf(index,:)))
    
    %% MF için invalid test data gelmesi durumunun çözümü
    while length(predicted_index_using_mf)>1
        disp('Invalid MF test data');
        return;
    end
    
    predictedArray(index,1)=predicted_index_using_mf;
    predictedArray(index,2)=class_wifi;
    
end


