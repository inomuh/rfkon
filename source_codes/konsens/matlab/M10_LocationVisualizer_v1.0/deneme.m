% datestr(now)
% xlswrite('D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.0\deneme.xlsx',datestr(now));
% x=12;
% xlswrite('D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.0\deneme.xlsx',x);

filename='Features.xlsx';
N='Adnan'; a=22; roll=22; gpa=3.55;
fileExist = exist(filename,'file'); 
if fileExist==0
 header = {'Name', 'age ','roll' , 'gpa'};
xlswrite(filename,header);


else

[~,~,input] = xlsread(filename); % Read in your xls file to a cell array (input)
new_data = {N, a,roll , gpa}; % This is a cell array of the new line you want to add
output = cat(1,input,new_data); % Concatinate your new data to the bottom of input
xlswrite(filename,output); % Write to the new excel file. 

end