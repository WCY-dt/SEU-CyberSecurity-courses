function [pcode] = pCode(satID,startTime)
% Ôóíêöèÿ ãåíåðèðóåò P-êîä ñèñòåìû GPS äëÿ çàäàííîãî ÊÀ, íà÷èíàÿ ñ çàäàííîé
% ñåêóíäû íåäåëè GPS è çàäàííîé äëèòåëüíîñòè. Àðãóìåíòû ôóíêöèè:
% satID     - PRN àïïàðàòà GPS
% startTime - Ñåêóíäà íåäåëè GPS, íà÷èíàÿ ñ êîòîðîé áóäåò ãåíåðèðîâàòüñÿ êîä (ïîêà òîëüêî êðàòíàÿ 1.5)
% Â äàëüíåéøåì ïëàíèðóåòñÿ:
% 1) Âøèòü â ôóíêöèþ âîçìîæíîñòü íà÷èíàòü êîä ñ ëþáîãî ìîìåíòà âðåìåíè
% 2) Äîáàâèòü âîçìîæíîñòü ãåíåðàöèè êîäà äëèííåå 1 ýïîõè


fd = 10.23e6;                                                        % ×àñòîòà äèñêðåòèçàöèè
duration = 1.5;                                                      % Ïîêà óìååì ãåíåðèðîâàòü òîëüêî îäíó ýïîõó

% Íîìåð òåêóùåé ýïîõè
currentEpoch = floor(startTime/1.5)+1;                               % Âû÷èñëåíèå íîìåðà òåêóùåé ýïîõè  

% Ãåíåðàöèÿ ïîñëåäîâàòåëüíîñòè X1A
register = logical([0 0 0 1 0 0 1 0 0 1 0 0]);                    
for i = 1:4092
    %Ñäâèãîâûé ðåãèñòð X1A: ãåíåðàöèÿ íîâîãî ýëåìåíòà ïîðîæäàþùèì ïîëèíîìîì è ñäâèã ðåãèñòðà
    X1A(i)      = register(end);
    newFirst    = xor(register(6),xor(register(8),xor(register(11),register(12))));
    register    = circshift(register,1);
    register(1) = newFirst;
end
X1A_epoch = repmat(X1A,1,3750);

% Ãåíåðàöèÿ ïîñëåäîâàòåëüíîñòè X1B
register = logical([0 0 1 0 1 0 1 0 1 0 1 0]); 
for i = 1:4093
    %Ñäâèãîâûé ðåãèñòð X1B: ãåíåðàöèÿ íîâîãî ýëåìåíòà ïîðîæäàþùèì ïîëèíîìîì è ñäâèã ðåãèñòðà
    X1B(i)      = register(end);
    newFirst    = xor(register(1),xor(register(2),xor(register(5),xor(register(8),xor(register(9),xor(register(10),xor(register(11),register(12))))))));
    register    = circshift(register,1);
    register(1) = newFirst;
end
X1B_epoch = [repmat(X1B,1,3749) ones(1,343)];   % Çà 343 ÷èïà äî êîíöà ýïîõè, X1B óäåðæèâàåòñÿ â ïîñëåäíåì ñîñòîÿíèè 

% Ãåíåðàöèÿ ïîñëåäîâàòåëüíîñòè X2A
register =  logical([1 0 1 0 0 1 0 0 1 0 0 1]); 
for i = 1:4092
    %Ñäâèãîâûé ðåãèñòð X1A: ãåíåðàöèÿ íîâîãî ýëåìåíòà ïîðîæäàþùèì ïîëèíîìîì è ñäâèã ðåãèñòðà
    X2A(i)      = register(end);
    newFirst    = xor(register(1),xor(register(3),xor(register(4),xor(register(5),xor(register(7),xor(register(8),xor(register(9),xor(register(10),xor(register(11),register(12))))))))));
    register    = circshift(register,1);
    register(1) = newFirst;
end
X2A_epoch = [repmat(X2A,1,3750) ones(1,37)];

% Ãåíåðàöèÿ ïîñëåäîâàòåëüíîñòè X2B
register = logical([0 0 1 0 1 0 1 0 1 0 1 0]); 
for i = 1:4093
    %Ñäâèãîâûé ðåãèñòð X1A: ãåíåðàöèÿ íîâîãî ýëåìåíòà ïîðîæäàþùèì ïîëèíîìîì è ñäâèã ðåãèñòðà
    X2B(i)      = register(end);
    newFirst    = xor(register(2),xor(register(3),xor(register(4),xor(register(8),xor(register(9),register(12))))));
    register    = circshift(register,1);
    register(1) = newFirst;
end
X2B_epoch = [repmat(X2B,1,3749) zeros(1,380)];  % Çà 343 ÷èïà äî êîíöà ýïîõè, X2B óäåðæèâàåòñÿ â ïîñëåäíåì ñîñòîÿíèè 

X1 = xor(X1A_epoch,X1B_epoch);   % Ãåíåðàöèÿ îäíîé ýïîõè ïîñëåäîâàòåëüíîñòè X1
X2 = xor(X2A_epoch,X2B_epoch);   % Ãåíåðàöèÿ îäíîé ýïîõè ïîñëåäîâàòåëüíîñòè X2

% Ôîðìèðîâàíèå X2i äëÿ êîíêðåòíîãî PRN

if X2(end) == 0
    X2i = circshift([X2 zeros(1,37)], satID);
else
    X2i = circshift([X2  ones(1,37)], satID);
end

if X2(1) == 0
    X2i(1:satID) = zeros(1,satID);
else
    X2i(1:satID) = ones(1,satID);
end

% !!Âîò òóò êîñòûëü, íàäî áóäåò åãî îáñóäèòü. Ïî÷åìó-òî äëÿ ïåðâîé ýïîõè
% íåäåëè íóæíî äðóãîå ñìåùåíèå X2 îòíîñèòåëüíî X1. Ñêîðåå âñåãî, åãî íîãè
% ðàñòóò â êðèâîì îïðåäåëåíèè íîìåðà òåêóùåé ýïîõè (ñì. ñòðîêó 15)
if currentEpoch==1
    X2i = circshift(X2i,37*(currentEpoch-1));
else
    X2i = circshift(X2i,37*(currentEpoch-0));
end

X1Pack = repmat(X1,1,ceil(duration/1.5));   % "Ïàêåò" X1 òðåáóåìîé äëèòåëüíîñòè                     
X2Pack = repmat(X2i,1,ceil(duration/1.5));  % "Ïàêåò" X2 òðåáóåìîé äëèòåëüíîñòè
X2Pack = X2Pack(1:length(X1Pack));          % Ïðèâåäåíèå ïàêåòà êîäîâ X2 ê äëèíå Õ1

pcode = xor(X1Pack,X2Pack);
end