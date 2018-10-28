%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This code is used for computing the CRC of a given hex message.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

close all
clear all
clc

hex_codeword_string = '0411';   % The codeword for which CRC is to be calculated (one string in hex format, like '0ABD16'). 
hex_CRC_polynomial = '8005';    % The CRC polynomial.

% Converting the message and CRC polynomial to binary format.
binary_codeword_string = hexToBinaryVector(hex_codeword_string);
binary_CRC_polynomial = hexToBinaryVector(hex_CRC_polynomial);

hGen = comm.CRCGenerator(binary_CRC_polynomial);    % Generating a cyclic code generator object.
codeword = step(hGen, binary_codeword_string');   % Computing the CRC polynomial

hex_CRC = binaryVectorToHex(codeword(length(binary_codeword_string)+1:end)'); % Converting the computed CRC to hex format.
msgbox(['The computed CRC is " ', hex_CRC,' "']);  % Showing the result in a message box.



