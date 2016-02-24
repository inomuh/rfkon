function plotRefPoints(actual,predicted)
    %Harita bilgileri
    X= [[8.67 8.67];
        [8.67 8.88];
        [8.88 8.88];
        [8.88 -11.36];
        [-11.36 -11.36];
        [-11.36 -8.02];
        [-6.98 -6.4];
        [-5.36 -1.17];
        [-0.13 4.05];
        [5.09 8.88];
        [7.65 7.65];
        [7.65 8.88];    
        [-1.37 -1.37];
        [3.73 3.73];
        [-11.36 6.64];
        [6.5 6.5];
        [3.76 6.5];
        [-0.22 3.76];
        [-5.35 -1.26];
        [-6.97 -6.39];
        [-11.36 -8.01];
        [-6.6 -6.6];
        [-1.52 -1.52];
        [3.76 3.76];
        [-6.6 -6.6];
        [6.45 6.85];
        [6.85 6.85];
        [6.85 6.45];
        [6.45 6.45]];

    Y= [[11.46 12.89];
        [11.46 11.46];
        [11.46 -7.53];
        [-7.53 -7.53];
        [-7.53 11.28];
        [0 0];
        [0 0];
        [0 0];
        [0 0];
        [0 0];
        [0 4.80];
        [4.80 4.80];    
        [0 -7.53];
        [0 -7.53];
        [11.28 11.28];
        [6.07 12.933];
        [6.77 6.77];
        [2.273 2.273];
        [2.273 2.273];
        [2.273 2.273];
        [2.273 2.273];
        [2.273 11.28];
        [2.273 11.28];
        [2.273 11.28];
        [0 -7.53];
        [2.00 2.00];
        [2.00 2.40];
        [2.40 2.40];
        [2.40 2.00]];

    minX=min(X)';
    onesX=minX*ones([1 length(X)]);
    onesX=onesX';
    X=X-onesX;

    minY=min(Y)';
    onesY=minY*ones([1 length(Y)]);
    onesY=onesY';
    Y=Y-onesY;


    figure(1);

    % Harita cizdir
     for k = 1:29
         line(X(k,:),Y(k,:), 'LineWidth',3, 'Color', [.0 .0 .0]);
         hold on
     end
     hold on

     %%
    %load RefPoints
    data=xlsread('C:\Users\sinem\Desktop\masaüstü\Dataset_Construction_PlotPoints\RefPointCoordinates');
    Points=[data(actual,1),data(actual,2),data(actual,3)];
    Points=[Points;data(predicted,1),data(predicted,2),data(predicted,3)];

    %%
%     for i = 1:20
%         plot(Points(i,2), Points(i,3), 'r*')
%         text(Points(i,2), Points(i,3), num2str(Points(i,1)), 'VerticalAlignment','bottom', ...
%                                  'HorizontalAlignment','left')
%     end
    plot(Points(1,2), Points(1,3), 'r*')
    text(Points(1,2), Points(1,3), num2str(Points(1,1)), 'VerticalAlignment','bottom', ...
                         'HorizontalAlignment','left')
    plot(Points(2,2), Points(2,3), 'bo')
    text(Points(2,2), Points(2,3), num2str(Points(2,1)), 'VerticalAlignment','bottom', ...
                         'HorizontalAlignment','left')

end
