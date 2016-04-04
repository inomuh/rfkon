function CalculateAccuracy_Sensor(testTmp_wifi,testTmp_mf,database,WiFi,MF,WiFi_MF,trainTmp_wifi)
    refPointNumber=size(database,1);
    accessPointNumber=size(trainTmp_wifi,2)-1;
    test_wifi=[];
    for i=1:refPointNumber
        test_wifi=[test_wifi;testTmp_wifi((i-1)*80+1:i*80-40,:)];
    end
    
    %% mf data is calibrated according to orientation of the mobile device
    test_mf = [];
%     for i = 1:length(testTmp_mf)
%         C = calibrated_mf([0; 0; 85.27], testTmp_mf(i,1:3)');
%         test_mf = [test_mf; C'];
%     end
    %% not calibrated mf data
    test_mf=testTmp_mf(:,1:3);
    
    %%
    [rowTestWiFiData columnTestWiFiData]=size(test_wifi);
    predictedArray=zeros(rowTestWiFiData,2);
    %nm: likelihood fonksiyonunu kullanarak her bir test datan�n t�m mean'i
    %hesaplanm�� train datalar�n birbirleriyle olan olas�l�klar�n� hesapl�yor
    nm_wifi=zeros(size(test_wifi,1),refPointNumber);
    nm_mf=zeros(size(test_wifi,1),refPointNumber);
    %confusion_matrix: weka'daki confusion matrix
    confusion_matrix=zeros(refPointNumber,refPointNumber);

    
    if WiFi_MF
        dataSize=size(test_wifi,1);
        for index=1:dataSize
            [BB,II] = sort(test_wifi(index,1:columnTestWiFiData-1)','ascend');
            B=BB';
            I=II';
            tmp=B;
            %testmean_sorted: s�ralanm�� elemanlardan nan olmayanlar� al�r
            %testmean_sorted_index: testmean_sorted indexlerini tutuyor
            test_sorted_wifi=B(~isnan(B));
            test_sorted_index_wifi=I(:,1:length(test_sorted_wifi));

            trainmean_wifi=[];
            trainstd_wifi=[];
            for i=1:length(test_sorted_wifi)
                trainmean_wifi=[trainmean_wifi,database(:,test_sorted_index_wifi(i)+2)];
                trainstd_wifi=[trainstd_wifi,database(:,test_sorted_index_wifi(i)+(2+accessPointNumber))];
            end


            for cn2=1:refPointNumber
                k1=1./(trainstd_wifi(cn2,:)*((2*pi)^0.5));
                k2=(test_sorted_wifi-trainmean_wifi(cn2,:)).^2;
                k3=2*(trainstd_wifi(cn2,:).^2);
                k4=k1.*exp(-1*(k2./k3));
                nm_wifi(index,cn2)=prod(k4);
        %         nm(cnx,cn2)=prod(log10(1./(trainstd_sorted(cn2,:).*sqrt(2*pi))).*exp((-(testmean_sorted(cnx,:)-trainmean_sorted(cn2,:)).^2)./));
            end

            class_wifi=test_wifi(index,columnTestWiFiData);
            maxIndex_wifi=find(nm_wifi(index,:)==max(nm_wifi(index,:)))

            %%%%%%%% Manyetik Alan ile konumland�rma do�rulu�unu iyile�tirme  %%%%%%%%

            predicted_refpoint_using_wifi=maxIndex_wifi;

            trainmean_mf=[];
            trainstd_mf=[];

            from=size(database,2)-(2*size(test_mf,2)-1);
            to=size(database,2)-size(test_mf,2);
            trainmean_mf=database(:,from:to);
            trainstd_mf=database(:,(to+1):size(database,2));

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

            for cn1=1:length(temp_komsuluk_wo_zeros_mf)        
                k1=1./(sub_trainstd_mf(cn1,:)*((2*pi)^0.5));
                k2=(sub_trainmean_mf(cn1,:)-test_mf(index,:)).^2;
                k3=2*(sub_trainstd_mf(cn1,:).^2);
                k4=k1.*exp(-1*(k2./k3));
                nm_mf(index,temp_komsuluk_wo_zeros_mf(1,cn1))=prod(k4);
            end
            
            predicted_index_using_mf=find(nm_mf(index,:)== max(nm_mf(index,:)))
            
            if class_wifi == predicted_index_using_mf
                confusion_matrix(class_wifi,class_wifi)=confusion_matrix(class_wifi,class_wifi)+1;        
            else       
                confusion_matrix(class_wifi,predicted_index_using_mf)=confusion_matrix(class_wifi,predicted_index_using_mf)+1;
            end

            predictedArray(index,1)=predicted_index_using_mf;
            predictedArray(index,2)=class_wifi;
        end
        csvwrite('predictedArray_WiFi_MF.csv',predictedArray);
        csvwrite('confusionMatrix_WiFi_MF.csv',confusion_matrix);
        sumTotal=sum(confusion_matrix(:));
        sumDiagonal=sum(diag(confusion_matrix));
        accuracy_WiFi_MF=sumDiagonal/sumTotal*100;
        save accuracy_WiFi_MF
    end
    
    if WiFi
        dataSize=size(test_wifi,1);
        for index=1:dataSize
            [BB,II] = sort(test_wifi(index,1:columnTestWiFiData-1)','ascend');
            B=BB';
            I=II';
            tmp=B;
            %testmean_sorted: s�ralanm�� elemanlardan nan olmayanlar� al�r
            %testmean_sorted_index: testmean_sorted indexlerini tutuyor
            test_sorted_wifi=B(~isnan(B));
            test_sorted_index_wifi=I(:,1:length(test_sorted_wifi));

            trainmean_wifi=[];
            trainstd_wifi=[];
            for i=1:length(test_sorted_wifi)
                trainmean_wifi=[trainmean_wifi,database(:,test_sorted_index_wifi(i)+2)];
                trainstd_wifi=[trainstd_wifi,database(:,test_sorted_index_wifi(i)+(2+accessPointNumber))];
            end


            for cn2=1:20
                k1=1./(trainstd_wifi(cn2,:)*((2*pi)^0.5));
                k2=(test_sorted_wifi-trainmean_wifi(cn2,:)).^2;
                k3=2*(trainstd_wifi(cn2,:).^2);
                k4=k1.*exp(-1*(k2./k3));
                nm_wifi(index,cn2)=prod(k4);
        %         nm(cnx,cn2)=prod(log10(1./(trainstd_sorted(cn2,:).*sqrt(2*pi))).*exp((-(testmean_sorted(cnx,:)-trainmean_sorted(cn2,:)).^2)./));
            end

            class_wifi=test_wifi(index,columnTestWiFiData)
            maxIndex_wifi=find(nm_wifi(index,:)==max(nm_wifi(index,:)))

            if(size(maxIndex_wifi)>1)
                disp('unclassified WiFi data')
            end
            
            if class_wifi == maxIndex_wifi
                confusion_matrix(class_wifi,class_wifi)=confusion_matrix(class_wifi,class_wifi)+1;        
            else       
                confusion_matrix(class_wifi,maxIndex_wifi)=confusion_matrix(class_wifi,maxIndex_wifi)+1;
            end
            index
            predictedArray(index,1)=maxIndex_wifi;
            predictedArray(index,2)=class_wifi;
        end
        csvwrite('predictedArray_WiFi.csv',predictedArray);
        csvwrite('confusionMatrix_WiFi.csv',confusion_matrix);
        sumTotal=sum(confusion_matrix(:));
        sumDiagonal=sum(diag(confusion_matrix));
        accuracy_WiFi=sumDiagonal/sumTotal*100;
        save accuracy_WiFi
    end
    
     if MF
        dataSize=size(test_mf,1);
        for index=1:dataSize
            trainmean_mf=[];
            trainstd_mf=[];

            from=size(database,2)-(2*size(test_mf,2)-1);
            to=size(database,2)-size(test_mf,2);
            trainmean_mf=database(:,from:to);
            trainstd_mf=database(:,(to+1):size(database,2));

            for cn1=1:length(trainmean_mf)        
                k1=1./(trainstd_mf(cn1,:)*((2*pi)^0.5));
                k2=(trainmean_mf(cn1,:)-test_mf(index,:)).^2;
                k3=2*(trainstd_mf(cn1,:).^2);
                k4=k1.*exp(-1*(k2./k3));
                nm_mf(index,cn1)=prod(k4);
            end
            predicted_index_using_mf=find(nm_mf(index,:)== max(nm_mf(index,:)));
            
            class_mf=testTmp_mf(index,size(testTmp_mf,2));
            if class_mf == predicted_index_using_mf
                confusion_matrix(class_mf,class_mf)=confusion_matrix(class_mf,class_mf)+1;        
            else       
                confusion_matrix(class_mf,predicted_index_using_mf)=confusion_matrix(class_mf,predicted_index_using_mf)+1;
            end
            
            predictedArray(index,1)=predicted_index_using_mf;
            predictedArray(index,2)=testTmp_mf(index,size(testTmp_mf,2));
        end
        csvwrite('predictedArray_MF.csv',predictedArray);
        csvwrite('confusionMatrix_MF.csv',confusion_matrix);
        sumTotal=sum(confusion_matrix(:));
        sumDiagonal=sum(diag(confusion_matrix));
        accuracy_MF=sumDiagonal/sumTotal*100;
        save accuracy_MF
    end
    
    
  
end


