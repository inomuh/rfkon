%% Train Database Constructor
function DatabaseConstruction(refPointCoordinates,noRefPoints,trainTmp_wifi)
    trainmean_wifi=[];
    trainstd_wifi=[];

    for cn=1:noRefPoints
        trainmean_wifi=[trainmean_wifi;nanmean(trainTmp_wifi((cn-1)*80+1:cn*80,:))];
        trainstd_wifi=[trainstd_wifi;nanstd(trainTmp_wifi((cn-1)*80+1:cn*80,:))];
    end

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
    columNumberOfDatabase=(size(refPointCoordinates,2)-1)+(size(trainmean_wifi,2)-1)+(size(trainstd_wifi,2)-1);
    database=zeros(rowNumberOfDatabase,columNumberOfDatabase);

    database(:,1:(size(refPointCoordinates,2)-1))=[refPointCoordinates(:,2),refPointCoordinates(:,3)];

    for cn=1:noRefPoints
        database(cn,size(refPointCoordinates,2):(size(refPointCoordinates,2)+size(trainmean_wifi,2)-2))=trainmean_wifi(cn,1:(size(trainmean_wifi,2)-1));
        database(cn,((size(refPointCoordinates,2)+size(trainmean_wifi,2)-1):(size(refPointCoordinates,2)+size(trainmean_wifi,2)+size(trainstd_wifi,2)-3)))=trainstd_wifi(cn,1:(size(trainstd_wifi,2)-1));
    end

    csvwrite('database_train.csv',database);

end