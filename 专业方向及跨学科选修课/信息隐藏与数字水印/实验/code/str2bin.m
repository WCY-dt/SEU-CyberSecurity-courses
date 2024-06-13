function a = str2bin(s)
    % str2bin() Converts a string into binary
    % INPUTS
    %    s - Input string
    % OUTPUTS
    %    a - Output binary

    a = dec2bin(s, 8)';
    a = a(:)' - '0';

end
