clc;clear all;close all;
[filenameTestMF,pathnameTestMF]=uigetfile({'*.csv'},'MF Test File Selector','D:\RFKON_UB_SB\src\M7_Likelihood_MF_MeanTest_v1.0\datasets\MF');
testTmp_mf_path=strcat(pathnameTestMF,filenameTestMF);
test_mf=csvread(testTmp_mf_path,1,0);
cn=test_mf(1,size(test_mf,2));
sayac=cn;
i=1;
testmean_mf=[];
while i<=size(test_mf,1)
    index=0;
    testMFAll=0;
    while cn==sayac
        testMFAll=testMFAll+test_mf(i,:);       
        if cn==sayac
            index=index+1;
            i=i+1;
        end
        
        if i>size(test_mf,1) 
            break; 
        else
            cn=test_mf(i,size(test_mf,2));
        end
    end    
    testmean_mf=[testmean_mf;testMFAll/index];
    sayac=cn;
end