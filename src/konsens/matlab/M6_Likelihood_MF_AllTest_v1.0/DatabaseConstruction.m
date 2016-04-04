%% Train Database Constructor
function DatabaseConstruction(refPointCoordinates,noRefPoints,temp_train_mf)
    train_mf = [];
    %% calibrated mf data
%     for i = 1:length(temp_train_mf)
%         C = calibrated_mf([0; 0; 85.27], temp_train_mf(i,:)'); 
%         train_mf = [train_mf; C'];
%     end
    %% not calibrated mf data
    train_mf=temp_train_mf;

    trainmean_mf=[];
    trainstd_mf=[];

    for cn=1:noRefPoints
        trainmean_mf=[trainmean_mf;mean(train_mf((cn-1)*40+1:cn*40,:))];
        trainstd_mf=[trainstd_mf;std(train_mf((cn-1)*40+1:cn*40,:))];
    end

    trainmean_wifi=[];
    trainstd_wifi=[];

    

    % rowNumberOfDatabase: number of records in the constructed database for
    % train data data keeps x, y coordinates of ref points, mean RSS values of
    % train wifi data, standart deviation of train wifi data, mean of train MF data,
    % and standart deviation of train MF data
    rowNumberOfDatabase=noRefPoints;
    % columNumberOfDatabase: column number of constructed database for train
    % data
    % 2(x, y)+2*number column of  train WiFi Data (for without NaN mean and NaN standart
    % deviation)+2*number column of train MF Data (for mean and standart
    % deviation)
    columNumberOfDatabase=(size(refPointCoordinates,2)-1)+size(trainmean_mf,2)+size(trainstd_mf,2);
    database=zeros(rowNumberOfDatabase,columNumberOfDatabase);

    database(:,1:(size(refPointCoordinates,2)-1))=[refPointCoordinates(:,2),refPointCoordinates(:,3)];

    database(:,size(refPointCoordinates,2):(size(refPointCoordinates,2)+size(trainmean_mf,2)-1))=trainmean_mf;
    database(:,(size(refPointCoordinates,2)+size(trainmean_mf,2)):columNumberOfDatabase)=trainstd_mf;

    %%%%%%%%%%%%%%  �NEML� %%%%%%%%%%%%%
    %%% header'lari da ekle sonra

    csvwrite('database_train.csv',database);
%     size(trainTmp_wifi,2);
%     size(temp_train_mf,2);
%     size(database,2);
%     save database
end