classdef visualization_tool < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure                        matlab.ui.Figure
        TabGroup                        matlab.ui.container.TabGroup
        TimeDataTab                     matlab.ui.container.Tab
        ChooseDataPanel                 matlab.ui.container.Panel
        SNRListBoxLabel                 matlab.ui.control.Label
        SNRListBox                      matlab.ui.control.ListBox
        BandListBoxLabel                matlab.ui.control.Label
        BandListBox                     matlab.ui.control.ListBox
        DesiredFrequencyEditFieldLabel  matlab.ui.control.Label
        DesiredFrequencyEditField       matlab.ui.control.NumericEditField
        TimeEditFieldLabel              matlab.ui.control.Label
        TimeEditField                   matlab.ui.control.NumericEditField
        Label                           matlab.ui.control.Label
        Time2EditField                  matlab.ui.control.NumericEditField
        PlotzPanel                      matlab.ui.container.Panel
        UIAxes                          matlab.ui.control.UIAxes
        UIAxes2                         matlab.ui.control.UIAxes
        UIAxes4                         matlab.ui.control.UIAxes
        ChooseFilePanel                 matlab.ui.container.Panel
        FIlesListBoxLabel               matlab.ui.control.Label
        FIlesListBox                    matlab.ui.control.ListBox
        HistogramTab                    matlab.ui.container.Tab
        UIAxes3                         matlab.ui.control.UIAxes
        MeanPeakDetectionOffsetTimemsEditFieldLabel  matlab.ui.control.Label
        MeanPeakDetectionOffsetTimemsEditField  matlab.ui.control.NumericEditField
        MissedPeaksLabel                matlab.ui.control.Label
        MissedPeaksEditField            matlab.ui.control.NumericEditField
    end

    
    properties (Access = public)
        DATA % holds all data
        SUBSET
        SNR
        T1 
        T2
        BAND
        peakLocations
        frequencies
    end
    
    methods (Access = private)
        
        function results = plotresult(app)
            tt=app.SUBSET.t(app.T1:app.T2);
            app.UIAxes.XLim=[tt(1),tt(end)];
            app.UIAxes2.XLim=[tt(1),tt(end)];
            app.UIAxes4.XLim=[tt(1),tt(end)];
            plot(app.UIAxes,tt,app.SUBSET.x(app.T1:app.T2),tt,207*app.SUBSET.detection(app.T1:app.T2)+514)
            legend(app.UIAxes,{'Noisy Signal','Detection: Peak, Trough, Zero Cross'})
            switch app.BAND
                case 1
                    plot(app.UIAxes2,tt,app.SUBSET.b35p(app.T1:app.T2),tt,app.SUBSET.b35a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500)             
                case 2
                    plot(app.UIAxes2,tt,app.SUBSET.b55p(app.T1:app.T2),tt,app.SUBSET.b55a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
                case 3
                    plot(app.UIAxes2,tt,app.SUBSET.b75p(app.T1:app.T2),tt,app.SUBSET.b75a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
                case 4
                    plot(app.UIAxes2,tt,app.SUBSET.b95p(app.T1:app.T2),tt,app.SUBSET.b95a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
            end
            legend(app.UIAxes2,{'Phase','Amplitude','Triggering'})
            plot(app.UIAxes4,tt,app.SUBSET.best(app.T1:app.T2));
            app.getFreq();
            end
        
        
        function results = plotresult_just_band(app)
            tt=app.SUBSET.t(app.T1:app.T2);
            switch app.BAND
                case 1
                    plot(app.UIAxes2,tt,app.SUBSET.b35p(app.T1:app.T2),tt,app.SUBSET.b35a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500)             
                case 2
                    plot(app.UIAxes2,tt,app.SUBSET.b55p(app.T1:app.T2),tt,app.SUBSET.b55a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
                case 3
                    plot(app.UIAxes2,tt,app.SUBSET.b75p(app.T1:app.T2),tt,app.SUBSET.b75a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
                case 4
                    plot(app.UIAxes2,tt,app.SUBSET.b95p(app.T1:app.T2),tt,app.SUBSET.b95a(app.T1:app.T2),tt,app.SUBSET.trigger(app.T1:app.T2)*500) 
            end
            legend(app.UIAxes2,{'Phase','Amplitude','Triggering'})
        end
        
        function getFreq(app)
            ll1=app.peakLocations<app.T2;
            ll2=app.peakLocations>app.T1;
            ll3=ll1 & ll2;
            if any(ll3)
                for i=1:length(ll3)
                    if ll3(i)==1
                        t1=i;
                        ff=1/(app.SUBSET.t(app.peakLocations(t1+1))-app.SUBSET.t(app.peakLocations(t1)));
                        break
                    end
                end
            else
                fprintf('No frequency detected\n');
                ff=-1;
            end
            app.UIAxes.Title.String=sprintf('Frequency = %.2f',ff);
            app.DesiredFrequencyEditField.Value=ff;
        end
        end
    

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            %Generated Signals
            filez=dir('*.log');
            for i=1:length(filez)
                ff{i}=filez(i).name;
            end
            app.FIlesListBox.Items=ff;
            
            snr=[4,8,12,16];           
            app.SNRListBox.Items=string(snr);
            app.UIAxes.YLim=[0 1024];
            app.UIAxes2.YLim=[-514 514];
            app.UIAxes4.YLim=[-1 5];
            app.SNR = '4';
            app.SNRListBox.Value='4';
            
%             app.FIlesListBox.
        end

        % Value changed function: SNRListBox
        function SNRListBoxValueChanged(app, event)
            app.SNR = app.SNRListBox.Value;
            row=app.DATA.snr==str2double(app.SNR);
            app.SUBSET=app.DATA(row,:);
            filterpeaks=app.SUBSET.detection>1.5;
            range=10;
            missed=0;
            checkat=app.peakLocations;
            mat=zeros(length(checkat),1);
            matrow=1;
            for i=1:length(checkat)
                look=checkat(i);
                if look<range+1
                    start=1;
                    stop=look+range;
                else
                    start=look-range;
                    stop=look+range;
                end
                if any(filterpeaks(start:stop))
                    for j=start:stop
                        if filterpeaks(j)==1
                            diff=look-j;
                            mat(matrow)=diff;
                            matrow=matrow+1;
                            break
                        end
                    end
                else
%                     fprintf(':(\n');
                    missed=missed+1;
                end
            end
%             fprintf('Missed: %d\n',missed);
            for i=length(mat):-1:1
                if mat(i)~=0
                    mat=mat(1:i);
                    break
                end
            end
            mat=mat*1/2000*10^3;
            totalpeaks=size(mat,1);
            h=histogram(app.UIAxes3,mat);
%             morebins(h);
            app.MeanPeakDetectionOffsetTimemsEditField.Value=mean(mat);
            app.MissedPeaksEditField.Value=100*(missed/totalpeaks);
            app.UIAxes3.Title.String=sprintf('SNR = %d\n',str2double(app.SNR));
            plotresult(app);
        end

        % Value changed function: BandListBox
        function BandListBoxValueChanged(app, event)
            value = app.BandListBox.Value;
            switch value
                case '20-50'
                    app.BAND=1;
                case '40-70'
                    app.BAND=2;
                case '60-90'
                    app.BAND=3;
                case '80-110'
                    app.BAND=4;
            end
            plotresult_just_band(app);
        end

        % Value changed function: TimeEditField
        function TimeEditFieldValueChanged(app, event)
            oval=app.TimeEditField.Value;
            value = floor(app.TimeEditField.Value*2000);
            app.T1=value;
            if app.T2<=value
               app.T2=value+0.1*2000;
               app.Time2EditField.Value=oval+0.1;
            end
            app.plotresult();
        end

        % Value changed function: Time2EditField
        function Time2EditFieldValueChanged(app, event)
            oval=app.Time2EditField.Value;
            value = floor(app.Time2EditField.Value*2000);
            
            app.T2=value;
            if value<=app.T1
                app.T1=value-0.1*2000;
                app.TimeEditField.Value=oval-0.1;
            end
            app.plotresult();
        end

        % Value changed function: DesiredFrequencyEditField
        function DesiredFrequencyEditFieldValueChanged(app, event)
            value = app.DesiredFrequencyEditField.Value;
            fff=app.frequencies;
            for i=1:length(fff)
               if fff(i)>value
                    break
               end
            end
            disp(fff(i))
            app.T1=app.peakLocations(i)-200;
            app.T2=app.peakLocations(i)+200;
            app.TimeEditField.Value=app.T1/2000;
            app.Time2EditField.Value=app.T2/2000;
            
            
            
            app.plotresult();
        end

        % Value changed function: FIlesListBox
        function FIlesListBoxValueChanged(app, event)
% x_no=1;
% 
% b35p=2;
% b35a=3;
% 
% b55p=4;
% b55a=5;
% 
% b75p=6;
% b75a=7;
% 
% b95p=8;
% b95a=9;
% best_no=10;
% detection_no=11;
% amp_data=12:15;
% trial_no=16;

            
            value = app.FIlesListBox.Value;
            load('X.mat','X');
            
            %Teensy signals
            namez={'x','b35p','b35a','b55p','b55a','b75p','b75a','b95p','b95a','best','detection','amp0','amp1','amp2','amp3','snr','counter','trigger'}';
            dtypez={'int32','double','double','double','double','double','double','double','double','int32','int32','double','double','double','double','int32','int32','int32'};
            oo=delimitedTextImportOptions('VariableNames',namez,'DataLines',2,'VariableTypes',dtypez);
            data=readtable(value,oo);
            t=repmat((0:1/2000:X.time_len)',4,1);
            data=addvars(data,t,'Before','x');
            
            clean=[X.clean_4 ;X.clean_8 ;X.clean_12; X.clean_16];
            data=addvars(data,clean);
            
            [~,app.peakLocations]=findpeaks(data.clean(1:20001));
            
            fff=app.peakLocations*0;
            for i=1:length(app.peakLocations)-1
                fff(i)=1/(t(app.peakLocations(i+1))-t(app.peakLocations(i)));
            end
            app.frequencies=fff(1:end-1);
            
            app.DATA=data;
            if isempty(app.T1)
                app.T1=0.001*2000;app.TimeEditField.Value=0.001;
                app.T2=10*2000;app.Time2EditField.Value=10;
                 app.BAND=1;
            end
            row=app.DATA.snr==str2double(app.SNR);
            app.SUBSET=app.DATA(row,:);
            app.SNRListBoxValueChanged();
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.Position = [0 0 1000 700];
            app.UIFigure.Name = 'MATLAB App';

            % Create TabGroup
            app.TabGroup = uitabgroup(app.UIFigure);
            app.TabGroup.Position = [1 1 1000 700];

            % Create TimeDataTab
            app.TimeDataTab = uitab(app.TabGroup);
            app.TimeDataTab.Title = 'Time Data';

            % Create ChooseDataPanel
            app.ChooseDataPanel = uipanel(app.TimeDataTab);
            app.ChooseDataPanel.Title = 'Choose Data';
            app.ChooseDataPanel.FontWeight = 'bold';
            app.ChooseDataPanel.Position = [1 5 290 370];

            % Create SNRListBoxLabel
            app.SNRListBoxLabel = uilabel(app.ChooseDataPanel);
            app.SNRListBoxLabel.HorizontalAlignment = 'right';
            app.SNRListBoxLabel.Position = [26 321 31 22];
            app.SNRListBoxLabel.Text = 'SNR';

            % Create SNRListBox
            app.SNRListBox = uilistbox(app.ChooseDataPanel);
            app.SNRListBox.Items = {};
            app.SNRListBox.ValueChangedFcn = createCallbackFcn(app, @SNRListBoxValueChanged, true);
            app.SNRListBox.Position = [72 271 100 74];
            app.SNRListBox.Value = {};

            % Create BandListBoxLabel
            app.BandListBoxLabel = uilabel(app.ChooseDataPanel);
            app.BandListBoxLabel.HorizontalAlignment = 'right';
            app.BandListBoxLabel.Position = [21 216 34 22];
            app.BandListBoxLabel.Text = 'Band';

            % Create BandListBox
            app.BandListBox = uilistbox(app.ChooseDataPanel);
            app.BandListBox.Items = {'20-50', '40-70', '60-90', '80-110'};
            app.BandListBox.ValueChangedFcn = createCallbackFcn(app, @BandListBoxValueChanged, true);
            app.BandListBox.Position = [70 166 100 74];
            app.BandListBox.Value = '20-50';

            % Create DesiredFrequencyEditFieldLabel
            app.DesiredFrequencyEditFieldLabel = uilabel(app.ChooseDataPanel);
            app.DesiredFrequencyEditFieldLabel.HorizontalAlignment = 'right';
            app.DesiredFrequencyEditFieldLabel.Position = [21 128 110 22];
            app.DesiredFrequencyEditFieldLabel.Text = 'Desired Frequency:';

            % Create DesiredFrequencyEditField
            app.DesiredFrequencyEditField = uieditfield(app.ChooseDataPanel, 'numeric');
            app.DesiredFrequencyEditField.ValueChangedFcn = createCallbackFcn(app, @DesiredFrequencyEditFieldValueChanged, true);
            app.DesiredFrequencyEditField.Position = [146 128 100 22];

            % Create TimeEditFieldLabel
            app.TimeEditFieldLabel = uilabel(app.ChooseDataPanel);
            app.TimeEditFieldLabel.HorizontalAlignment = 'right';
            app.TimeEditFieldLabel.Position = [37 83 35 22];
            app.TimeEditFieldLabel.Text = 'Time:';

            % Create TimeEditField
            app.TimeEditField = uieditfield(app.ChooseDataPanel, 'numeric');
            app.TimeEditField.ValueChangedFcn = createCallbackFcn(app, @TimeEditFieldValueChanged, true);
            app.TimeEditField.Position = [87 83 40 22];

            % Create Label
            app.Label = uilabel(app.ChooseDataPanel);
            app.Label.HorizontalAlignment = 'right';
            app.Label.Position = [131 83 25 22];
            app.Label.Text = '-';

            % Create Time2EditField
            app.Time2EditField = uieditfield(app.ChooseDataPanel, 'numeric');
            app.Time2EditField.ValueChangedFcn = createCallbackFcn(app, @Time2EditFieldValueChanged, true);
            app.Time2EditField.Position = [171 83 52 22];

            % Create PlotzPanel
            app.PlotzPanel = uipanel(app.TimeDataTab);
            app.PlotzPanel.Title = 'Plotz';
            app.PlotzPanel.Position = [291 5 710 670];

            % Create UIAxes
            app.UIAxes = uiaxes(app.PlotzPanel);
            title(app.UIAxes, 'Signal and Detection')
            xlabel(app.UIAxes, 'Time')
            ylabel(app.UIAxes, 'Value')
            app.UIAxes.Position = [1 440 710 210];

            % Create UIAxes2
            app.UIAxes2 = uiaxes(app.PlotzPanel);
            title(app.UIAxes2, 'Filter Bank')
            xlabel(app.UIAxes2, 'Time')
            ylabel(app.UIAxes2, 'Value')
            app.UIAxes2.Position = [1 225 710 215];

            % Create UIAxes4
            app.UIAxes4 = uiaxes(app.PlotzPanel);
            title(app.UIAxes4, 'Detection bank')
            xlabel(app.UIAxes4, 'X')
            ylabel(app.UIAxes4, 'Bank Number')
            app.UIAxes4.Position = [1 5 710 215];

            % Create ChooseFilePanel
            app.ChooseFilePanel = uipanel(app.TimeDataTab);
            app.ChooseFilePanel.Title = 'Choose File';
            app.ChooseFilePanel.FontWeight = 'bold';
            app.ChooseFilePanel.Position = [1 375 290 300];

            % Create FIlesListBoxLabel
            app.FIlesListBoxLabel = uilabel(app.ChooseFilePanel);
            app.FIlesListBoxLabel.HorizontalAlignment = 'right';
            app.FIlesListBoxLabel.Position = [11 246 32 22];
            app.FIlesListBoxLabel.Text = 'FIles';

            % Create FIlesListBox
            app.FIlesListBox = uilistbox(app.ChooseFilePanel);
            app.FIlesListBox.Items = {};
            app.FIlesListBox.ValueChangedFcn = createCallbackFcn(app, @FIlesListBoxValueChanged, true);
            app.FIlesListBox.Position = [11 118 270 122];
            app.FIlesListBox.Value = {};

            % Create HistogramTab
            app.HistogramTab = uitab(app.TabGroup);
            app.HistogramTab.Title = 'Histogram';

            % Create UIAxes3
            app.UIAxes3 = uiaxes(app.HistogramTab);
            title(app.UIAxes3, 'Title')
            xlabel(app.UIAxes3, 'Delay Time (0 is true peak)')
            ylabel(app.UIAxes3, 'Number of Occurances')
            app.UIAxes3.Position = [30 145 761 505];

            % Create MeanPeakDetectionOffsetTimemsEditFieldLabel
            app.MeanPeakDetectionOffsetTimemsEditFieldLabel = uilabel(app.HistogramTab);
            app.MeanPeakDetectionOffsetTimemsEditFieldLabel.HorizontalAlignment = 'right';
            app.MeanPeakDetectionOffsetTimemsEditFieldLabel.Position = [91 83 213 22];
            app.MeanPeakDetectionOffsetTimemsEditFieldLabel.Text = 'Mean Peak Detection Offset Time (ms)';

            % Create MeanPeakDetectionOffsetTimemsEditField
            app.MeanPeakDetectionOffsetTimemsEditField = uieditfield(app.HistogramTab, 'numeric');
            app.MeanPeakDetectionOffsetTimemsEditField.Position = [319 83 100 22];

            % Create MissedPeaksLabel
            app.MissedPeaksLabel = uilabel(app.HistogramTab);
            app.MissedPeaksLabel.HorizontalAlignment = 'right';
            app.MissedPeaksLabel.Position = [101 43 102 22];
            app.MissedPeaksLabel.Text = 'Missed Peaks (%)';

            % Create MissedPeaksEditField
            app.MissedPeaksEditField = uieditfield(app.HistogramTab, 'numeric');
            app.MissedPeaksEditField.Position = [218 43 100 22];

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = visualization_tool

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            % Execute the startup function
            runStartupFcn(app, @startupFcn)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end