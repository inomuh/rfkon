function  dispcallback(obj, event)
data = fscanf(obj, '%c', get(obj, 'BytesAvailable'));

if(~isempty(data))
%     disp('parse ediliyor.');
    serverData = JSON.parse(data);
    fields=fieldnames(serverData);

    %%reset
    if strcmp(fields{1},'Reset')
%         disp('Komut reset');
        s=struct(fields{1},serverData.(fields{1}));
        s.Reset=true;
    end
    %%set refresh rate
    if strcmp(fields{1},'SetRefreshRate')
%         disp('Komut set refresh rate');
        s=struct(fields{1},serverData.(fields{1}),fields{3},serverData.(fields{3}),'Type','Set');
        s.Status=true;
    end
    %%get refresh rate
    if strcmp(fields{1},'GetRefreshRate')
%         disp('Komut get refresh rate');
        s=struct(fields{1},serverData.(fields{1}),fields{3},serverData.(fields{3}),'Type','Get'); 
        s.RefRate='XX';
        s.Status=true;
    end
    
    %%get net status
    if strcmp(fields{1},'IP')
%         disp('Komut get net status');
        s=struct(fields{1},serverData.(fields{1}),fields{2},serverData.(fields{2}),...,
        fields{3},serverData.(fields{3}),fields{4},serverData.(fields{4}),...,
        fields{5},serverData.(fields{5}),fields{7},serverData.(fields{7}));
        s.IP='XX';
        s.Subnet='XX';
        s.Gateway='XX';
        s.DNS='XX';
        s.Name='XX';
        s.Status=true;
    end

    %%get alive status
    if strcmp(fields{1},'Alive')
%         disp('Komut get alive status');
        s=struct(fields{1},serverData.(fields{1}));
        s.Alive=true;
    end

%     disp('Message deðiþti')
    dataX=savejson('',s);
    myNewData = strrep(dataX,sprintf('\n'),'');
    send_data = strcat(myNewData);
    fprintf(obj,send_data);
    
end

end

