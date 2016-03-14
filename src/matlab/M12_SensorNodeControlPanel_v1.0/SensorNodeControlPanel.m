function varargout = SensorNodeControlPanel(varargin)
% SENSORNODECONTROLPANEL MATLAB code for SensorNodeControlPanel.fig
%      SENSORNODECONTROLPANEL, by itself, creates a new SENSORNODECONTROLPANEL or raises the existing
%      singleton*.
%
%      H = SENSORNODECONTROLPANEL returns the handle to a new SENSORNODECONTROLPANEL or the handle to
%      the existing singleton*.
%
%      SENSORNODECONTROLPANEL('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SENSORNODECONTROLPANEL.M with the given input arguments.
%
%      SENSORNODECONTROLPANEL('Property','Value',...) creates a new SENSORNODECONTROLPANEL or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SensorNodeControlPanel_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SensorNodeControlPanel_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help SensorNodeControlPanel

% Last Modified by GUIDE v2.5 21-Oct-2015 15:35:12

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SensorNodeControlPanel_OpeningFcn, ...
                   'gui_OutputFcn',  @SensorNodeControlPanel_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before SensorNodeControlPanel is made visible.
function SensorNodeControlPanel_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SensorNodeControlPanel (see VARARGIN)

% Choose default command line output for SensorNodeControlPanel
handles.output = hObject;

set(handles.edit_SetRefreshRate,'enable','off');
set(handles.pb_OpenConnection,'enable','on');
set(handles.pb_CloseConnection,'enable','on');
set(handles.pb_Reset,'enable','off');
set(handles.pb_SetRefreshRate,'enable','off');
set(handles.pb_GetRefreshRate,'enable','off');
set(handles.pb_GetNetStatus,'enable','off');
set(handles.pb_GetAliveStatus,'enable','off');
set(handles.tb_Reset,'enable','off');
set(handles.tb_SetRefreshRate,'enable','off');
set(handles.tb_GetRefreshRate,'enable','off');
set(handles.tb_GetNetStatus,'enable','off');
set(handles.tb_GetAliveStatus,'enable','off');
set(handles.uipanel_GetNetStatus,'visible','off');
guidata(hObject, handles);

function localRead(interfaceObject,~,hObject,handles)
% disp('Data is reading from callback')

try
    sizeofincome = get(interfaceObject, 'BytesAvailable');
    handles.tcp_data = fscanf(interfaceObject, '%c', sizeofincome);
catch
    disp('ERROR in callback')
end
disp(handles.tcp_data)
if(~isempty(handles.tcp_data))
    rawData=char(handles.tcp_data);
    clientData = JSON.parse(rawData);
    fields=fieldnames(clientData);
    
%%reset
    if strcmp(fields{1},'Reset')             
        set(handles.tb_Reset,'BackgroundColor','green');
        set(handles.tb_SetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetNetStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetAliveStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
   
%%set refresh rate
    elseif strcmp(fields{1},'SetRefreshRate')
        set(handles.tb_SetRefreshRate,'BackgroundColor','green');
        set(handles.tb_Reset,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetNetStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetAliveStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
    
%%get refresh rate 
    elseif strcmp(fields{1},'GetRefreshRate')
        set(handles.text_GetRefreshRate,'String',clientData.RefRate);
        set(handles.tb_GetRefreshRate,'BackgroundColor','green');
        set(handles.tb_Reset,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_SetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetNetStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetAliveStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
     
%%get net status
    elseif strcmp(fields{1},'IP')
        set(handles.text_IP,'String',clientData.IP);
        set(handles.text_Subnet,'String',clientData.Subnet);
        set(handles.text_Gateway,'String',clientData.Gateway);
        set(handles.text_DNS,'String',clientData.DNS);
        set(handles.text_Hostname,'String',clientData.Name);
        set(handles.tb_GetNetStatus,'BackgroundColor','green');
        set(handles.tb_Reset,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_SetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetAliveStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
        
%%get alive status
    elseif strcmp(fields{1},'Alive')
        set(handles.text_Alive,'String','Alive');
        set(handles.tb_GetAliveStatus,'BackgroundColor','green');
        set(handles.tb_Reset,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_SetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
        set(handles.tb_GetNetStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
    else
        disp('Hatalý Komut');
    end
end
guidata(hObject, handles);

% --- Outputs from this function are returned to the command line.
function varargout = SensorNodeControlPanel_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function edit_NodeID_Callback(hObject, eventdata, handles)
% hObject    handle to edit_NodeID (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_NodeID as text
%        str2double(get(hObject,'String')) returns contents of edit_NodeID as a double


% --- Executes during object creation, after setting all properties.
function edit_NodeID_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_NodeID (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pb_Reset.
function pb_Reset_Callback(hObject, eventdata, handles)
% hObject    handle to pb_Reset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

try
    set(handles.tb_Reset,'BackgroundColor','yellow');
%     NodeID = get(handles.edit_NodeID,'String'); 
%     NodeID=strcat('SensDug',NodeID);
    Selected = get(handles.NodeIdSelection, 'Value');
    NodeID='';
    switch Selected
        case 1
            NodeID='SensDug11';
        case 2
            NodeID='SensDug12';
        case 3
            NodeID='SensDug13';
        case 4
            NodeID='SensDug15';
        case 5
            NodeID='SensDug16';
        case 6
            NodeID='SensDug25';
        case 7    
            NodeID='SensDug26';
    end
    str=strcat('{"Reset":false,"NodeID":"',NodeID,'"}');
    fprintf(handles.tcp_obj, str);
    guidata(hObject, handles);
catch
    set(handles.tb_Reset,'BackgroundColor','red');
end

% --- Executes on button press in pb_SetRefreshRate.
function pb_SetRefreshRate_Callback(hObject, eventdata, handles)
% hObject    handle to pb_SetRefreshRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

try
    set(handles.tb_SetRefreshRate,'BackgroundColor','yellow');
%     NodeID = get(handles.edit_NodeID,'String'); 
%     NodeID=strcat('SensDug',NodeID);
    Selected = get(handles.NodeIdSelection, 'Value');
    NodeID='';
    switch Selected
        case 1
            NodeID='SensDug11';
        case 2
            NodeID='SensDug12';
        case 3
            NodeID='SensDug13';
        case 4
            NodeID='SensDug15';
        case 5
            NodeID='SensDug16';
        case 6
            NodeID='SensDug25';
        case 7    
            NodeID='SensDug26';
    end
    RefRate=get(handles.edit_SetRefreshRate,'String');
    str=strcat('{"SetRefreshRate":"',RefRate,'","NodeID":"',NodeID,'","Status":false}');
    fprintf(handles.tcp_obj, str);
    guidata(hObject, handles);
catch
    set(handles.tb_SetRefreshRate,'BackgroundColor','yellow');
end


function edit_SetRefreshRate_Callback(hObject, eventdata, handles)
% hObject    handle to edit_SetRefreshRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_SetRefreshRate as text
%        str2double(get(hObject,'String')) returns contents of edit_SetRefreshRate as a double


% --- Executes during object creation, after setting all properties.
function edit_SetRefreshRate_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_SetRefreshRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pb_GetRefreshRate.
function pb_GetRefreshRate_Callback(hObject, eventdata, handles)
% hObject    handle to pb_GetRefreshRate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

try
    set(handles.tb_GetRefreshRate,'BackgroundColor','yellow');
%     NodeID = get(handles.edit_NodeID,'String'); 
%     NodeID=strcat('SensDug',NodeID);
    Selected = get(handles.NodeIdSelection, 'Value');
    NodeID='';
    switch Selected
        case 1
            NodeID='SensDug11';
        case 2
            NodeID='SensDug12';
        case 3
            NodeID='SensDug13';
        case 4
            NodeID='SensDug15';
        case 5
            NodeID='SensDug16';
        case 6
            NodeID='SensDug25';
        case 7    
            NodeID='SensDug26';
    end
    str=strcat('{"GetRefreshRate":"-1",','"NodeID":"',NodeID,'","Status":false}');
    fprintf(handles.tcp_obj, str);
    guidata(hObject, handles);
catch
    set(handles.tb_GetRefreshRate,'BackgroundColor','red');
end


% --- Executes on button press in pb_GetNetStatus.
function pb_GetNetStatus_Callback(hObject, eventdata, handles)
% hObject    handle to pb_GetNetStatus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
try
    set(handles.tb_GetNetStatus,'BackgroundColor','yellow');
%     NodeID = get(handles.edit_NodeID,'String'); 
%     NodeID=strcat('SensDug',NodeID);
    Selected = get(handles.NodeIdSelection, 'Value');
    NodeID='';
    switch Selected
        case 1
            NodeID='SensDug11';
        case 2
            NodeID='SensDug12';
        case 3
            NodeID='SensDug13';
        case 4
            NodeID='SensDug15';
        case 5
            NodeID='SensDug16';
        case 6
            NodeID='SensDug25';
        case 7    
            NodeID='SensDug26';
    end
    str=strcat('{"IP":"0","Subnet":"0","Gateway":"0","DNS":"0","Name":"0","NodeID":"',NodeID,'","Status":false}');
    fprintf(handles.tcp_obj, str);
    guidata(hObject, handles);
catch
    set(handles.tb_GetNetStatus,'BackgroundColor','red');
end

% --- Executes on button press in pb_GetAliveStatus.
function pb_GetAliveStatus_Callback(hObject, eventdata, handles)
% hObject    handle to pb_GetAliveStatus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


try
    set(handles.tb_GetAliveStatus,'BackgroundColor','yellow');
%     NodeID = get(handles.edit_NodeID,'String'); 
%     NodeID=strcat('SensDug',NodeID);

    Selected = get(handles.NodeIdSelection, 'Value');
    NodeID='';
    switch Selected
        case 1
            NodeID='SensDug11';
        case 2
            NodeID='SensDug12';
        case 3
            NodeID='SensDug13';
        case 4
            NodeID='SensDug15';
        case 5
            NodeID='SensDug16';
        case 6
            NodeID='SensDug25';
        case 7    
            NodeID='SensDug26';
    end
    str=strcat('{"Alive":false,','"NodeID":"',NodeID,'"}');
    fprintf(handles.tcp_obj, str);
    guidata(hObject, handles);
catch
    set(handles.tb_GetAliveStatus,'BackgroundColor','red');
end
% --- Executes on button press in pb_CloseConnection.
function pb_CloseConnection_Callback(hObject, eventdata, handles)
% hObject    handle to pb_CloseConnection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
try
    fclose(handles.tcp_obj);
    
    set(handles.edit_SetRefreshRate,'String','');
    set(handles.text_GetRefreshRate,'String','');
    set(handles.text_Alive,'String','');
    
    set(handles.edit_SetRefreshRate,'enable','off');
    set(handles.pb_OpenConnection,'enable','on');
    set(handles.pb_CloseConnection,'enable','on');
    set(handles.pb_Reset,'enable','off');
    set(handles.pb_SetRefreshRate,'enable','off');
    set(handles.pb_GetRefreshRate,'enable','off');
    set(handles.pb_GetNetStatus,'enable','off');
    set(handles.pb_GetAliveStatus,'enable','off');
    set(handles.tb_Reset,'enable','off');
    set(handles.tb_SetRefreshRate,'enable','off');
    set(handles.tb_GetRefreshRate,'enable','off');
    set(handles.tb_GetNetStatus,'enable','off');
    set(handles.tb_GetAliveStatus,'enable','off');
    set(handles.uipanel_GetNetStatus,'visible','off');
    set(handles.tb_Reset,'BackgroundColor',[0.9412 0.9412 0.9412]);
    set(handles.tb_SetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
    set(handles.tb_GetRefreshRate,'BackgroundColor',[0.9412 0.9412 0.9412]);
    set(handles.tb_GetNetStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
    set(handles.tb_GetAliveStatus,'BackgroundColor',[0.9412 0.9412 0.9412]);
    guidata(hObject, handles);
catch
    disp('Connection closing error')
end

% --- Executes on button press in pb_OpenConnection.
function pb_OpenConnection_Callback(hObject, eventdata, handles)
% hObject    handle to pb_OpenConnection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
try
    handles.tcp_obj = tcpip('localhost', 30000, 'NetworkRole', 'client');
%     disp('1')
    set(handles.tcp_obj, 'ReadAsyncMode', 'continuous');
    handles.tcp_obj.BytesAvailableFcn = {@localRead, hObject, handles};
    fopen(handles.tcp_obj);
    disp('TCP/IP Connection is opened');
    
    set(handles.edit_SetRefreshRate,'String','');
    set(handles.text_GetRefreshRate,'String','');
    set(handles.text_Alive,'String','');
    
    set(handles.text_IP,'String','');
    set(handles.text_Subnet,'String','');
    set(handles.text_Gateway,'String','');
    set(handles.text_DNS,'String','');
    set(handles.text_Hostname,'String','');
    
    set(handles.edit_SetRefreshRate,'Enable','on');
    set(handles.pb_OpenConnection,'enable','on');
    set(handles.pb_CloseConnection,'Enable','on');
    set(handles.pb_Reset,'Enable','on');
    set(handles.pb_SetRefreshRate,'Enable','on');
    set(handles.pb_GetRefreshRate,'Enable','on');
    set(handles.pb_GetNetStatus,'Enable','on');
    set(handles.pb_GetAliveStatus,'Enable','on');
    set(handles.tb_Reset,'Enable','on');
    set(handles.tb_SetRefreshRate,'Enable','on');
    set(handles.tb_GetRefreshRate,'Enable','on');
    set(handles.tb_GetNetStatus,'Enable','on');
    set(handles.tb_GetAliveStatus,'Enable','on');
    set(handles.uipanel_GetNetStatus,'visible','on');
    guidata(hObject, handles);
catch
    disp('Connection opening error')
end


% --- Executes on selection change in NodeIdSelection.
function NodeIdSelection_Callback(hObject, eventdata, handles)
% hObject    handle to NodeIdSelection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns NodeIdSelection contents as cell array
%        contents{get(hObject,'Value')} returns selected item from NodeIdSelection


% --- Executes during object creation, after setting all properties.
function NodeIdSelection_CreateFcn(hObject, eventdata, handles)
% hObject    handle to NodeIdSelection (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
