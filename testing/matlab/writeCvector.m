function writeCvector(name,data,type)

switch type
    case 'double'
        %% makes a double vector
        fid = fopen(sprintf('../%s.h',name),'w');
        l=length(data);
        fprintf(fid,'#define LEN %d\n',l);
        fprintf(fid,'//Vector created from matlab\n\ndouble %s[%d] = { %.15f',name,l,data(1));
        for i=2:l
           fprintf(fid,' , %.15f',data(i)); 
        end
        fprintf(fid,'};\n');
        fclose(fid);
    case 'int'
        %% makes an int vector
        fid = fopen(sprintf('../%s.h',name),'w');
        l=length(data);
        fprintf(fid,'#define LEN %d\n',l);
        fprintf(fid,'//Vector created from matlab\n\nint %s[%d] = { %d',name,l,data(1));
        for i=2:l
           fprintf(fid,', %d',data(i)); 
        end
        fprintf(fid,'};\n');
        fclose(fid);
    otherwise
        fprintf('Not a valid type\n');
end
    
end

