function varargout = RFKONLocalizer(varargin)
% RFKONLOCALIZER MATLAB code for RFKONLocalizer.fig
%      RFKONLOCALIZER, by itself, creates a new RFKONLOCALIZER or raises the existing
%      singleton*.
%
%      H = RFKONLOCALIZER returns the handle to a new RFKONLOCALIZER or the handle to
%      the existing singleton*.
%
%      RFKONLOCALIZER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in RFKONLOCALIZER.M with the given input arguments.
%
%      RFKONLOCALIZER('Property','Value',...) creates a new RFKONLOCALIZER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before RFKONLocalizer_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to RFKONLocalizer_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help RFKONLocalizer

% Last Modified by GUIDE v2.5 06-Mar-2016 22:18:24

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @RFKONLocalizer_OpeningFcn, ...
                   'gui_OutputFcn',  @RFKONLocalizer_OutputFcn, ...
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



% --- Executes just before RFKONLocalizer is made visible.
function RFKONLocalizer_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to RFKONLocalizer (see VARARGIN)

handles.X= [[8.67 8.67];
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

 handles.Y= [[11.46 12.89];
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

    handles.minX=min(handles.X)';
    handles.onesX=handles.minX*ones([1 length(handles.X)]);
    handles.onesX=handles.onesX';
    handles.X=handles.X-handles.onesX;

    handles.minY=min(handles.Y)';
    handles.onesY=handles.minY*ones([1 length(handles.Y)]);
    handles.onesY=handles.onesY';
    handles.Y=handles.Y-handles.onesY;
  
    axes(handles.axes1);
    % figure('name','Teknopark Floor 1 Layout');

    % Harita cizdir
     for k = 1:29
         line(handles.X(k,:),handles.Y(k,:), 'LineWidth',3, 'Color', [.0 .0 .0]);
         hold on
     end
     
     handles.refPointCoordinates=csvread('D:\RFKON_UB_SB\src\M10_LocationVisualizer_v1.1\RefPointCoordinates.csv',1,0)
     handles.Points=[];
     for i=1:size(handles.refPointCoordinates,1)
         handles.Point=[handles.refPointCoordinates(i,1),handles.refPointCoordinates(i,2),handles.refPointCoordinates(i,3)];
         handles.Points=[handles.Points;handles.Point];
         plot(handles.Points(i,2), handles.Points(i,3), 'k*');
         text(handles.Points(i,2), handles.Points(i,3), num2str(handles.Points(i,1)), 'VerticalAlignment','bottom', ...
                         'HorizontalAlignment','left');
         handles.output = hObject;
         guidata(hObject, handles);
     end
     
     hold on

     
     
% Choose default command line output for RFKONLocalizer
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes RFKONLocalizer wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = RFKONLocalizer_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.output = hObject;
guidata(hObject, handles);
handles.testDataSizeIncrementer=Likelihood(hObject, eventdata, handles);
%set(handles.text2, 'String', num2str(handles.testDataSizeIncrementer)) ;
handles.output = hObject;
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function text2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to text2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% set(handles.text3, 'String', '') ;
% delete(handles.text3)
% delete(handles.figure1)
% delete(hObject)
% guidata(hObject,handles);
% handles = guidata(hObject);

% figure1_CloseRequestFcn(hObject, eventdata, handles)


% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: delete(hObject) closes the figure

delete(handles.text3);
delete(handles.figure1)
% delete(hObject);
% guidata(hObject, handles);
