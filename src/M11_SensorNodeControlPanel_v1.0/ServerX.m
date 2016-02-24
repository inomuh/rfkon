tcp_obj = tcpip('localhost', 30000, 'NetworkRole', 'server');
% handles.tcp_obj.BytesAvailableFcn = {@localRead, hObject, handles};
set(tcp_obj, 'ReadAsyncMode', 'continuous');

set(tcp_obj, 'BytesAvailableFcn', {'dispcallback'});

fopen(tcp_obj);

get(tcp_obj, 'Status')
% disp(data);
% fprintf(tcp_obj,data);