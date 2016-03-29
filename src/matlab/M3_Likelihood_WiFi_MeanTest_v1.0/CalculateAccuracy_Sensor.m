function CalculateAccuracy_Sensor(testTmp_wifi,database,trainTmp_wifi)
    refPointNumber=size(database,1);
    accessPointNumber=size(trainTmp_wifi,2)-1;
    test_wifi=testTmp_wifi;
    %%
    [rowTestWiFiData columnTestWiFiData]=size(test_wifi);
    predictedArray=zeros(rowTestWiFiData,2);
    %nm: likelihood fonksiyonunu kullanarak her bir test datan�n t�m mean'i
    %hesaplanm�� train datalar�n birbirleriyle olan olas�l�klar�n� hesapl�yor
    nm_wifi=zeros(size(test_wifi,1),refPointNumber);
    nm_mf=zeros(size(test_wifi,1),refPointNumber);
    %confusion_matrix: weka'daki confusion matrix
    confusion_matrix=zeros(refPointNumber,refPointNumber);

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
        else

            if class_wifi == maxIndex_wifi
                confusion_matrix(class_wifi,class_wifi)=confusion_matrix(class_wifi,class_wifi)+1;        
            else       
                confusion_matrix(class_wifi,maxIndex_wifi)=confusion_matrix(class_wifi,maxIndex_wifi)+1;
            end
    %         index
            predictedArray(index,1)=maxIndex_wifi;
            predictedArray(index,2)=class_wifi;
        end
    end
    csvwrite('predictedArray_WiFi.xlsx',predictedArray);
    csvwrite('confusionMatrix_WiFi.xlsx',confusion_matrix);
    sumTotal=sum(confusion_matrix(:));
    sumDiagonal=sum(diag(confusion_matrix));
    accuracy_WiFi=sumDiagonal/sumTotal*100;
    save accuracy_WiFi
end


