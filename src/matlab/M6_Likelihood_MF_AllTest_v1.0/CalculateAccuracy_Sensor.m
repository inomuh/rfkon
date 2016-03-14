function CalculateAccuracy_Sensor(testTmp_mf,database)
    refPointNumber=size(database,1);
    
%         test_mf = [];
%     for i = 1:length(testTmp_mf)
%         C = calibrated_mf([0; 0; 85.27], testTmp_mf(i,1:3)');
%         test_mf = [test_mf; C'];
%     end
    %% not calibrated mf data
    test_mf=testTmp_mf(:,1:3);
    

    nm_mf=zeros(size(test_mf,1),refPointNumber);
    %confusion_matrix: weka'daki confusion matrix
    confusion_matrix=zeros(refPointNumber,refPointNumber);

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
        size(predicted_index_using_mf,1)
        if size(predicted_index_using_mf,2)>1
            disp('Unclassified MF data')
        else
            predicted_index_using_mf
        class_mf=testTmp_mf(index,size(testTmp_mf,2))
        if class_mf == predicted_index_using_mf
            confusion_matrix(class_mf,class_mf)=confusion_matrix(class_mf,class_mf)+1;        
        else       
            confusion_matrix(class_mf,predicted_index_using_mf)=confusion_matrix(class_mf,predicted_index_using_mf)+1;
        end

        predictedArray(index,1)=predicted_index_using_mf;
        predictedArray(index,2)=testTmp_mf(index,size(test_mf,2));
        end
    end
    xlswrite('D:\RFKON_UB_SB\src\M6_Likelihood_MF_MeanTest_v1.0\predictedArray_MF_wthRotate.xlsx',predictedArray);
    xlswrite('D:\RFKON_UB_SB\src\M6_Likelihood_MF_MeanTest_v1.0\confusionMatrix_MF_wthRotate.xlsx',confusion_matrix);
    sumTotal=sum(confusion_matrix(:));
    sumDiagonal=sum(diag(confusion_matrix));
    accuracy_MF=sumDiagonal/sumTotal*100;
    save accuracy_MF
end

