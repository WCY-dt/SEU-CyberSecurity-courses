clear;
clc;
fid=fopen('隐藏.mid','r');
[wa,length]=fread(fid,inf,'uint8');
getfid=fopen('提取.txt','a');
fclose(fid);
count=72;
p=1;
for i=1:length
    if(bitand(wa(i,1),240)==144)&&(p<=count)
        fwrite(getfid,bitget(wa(i+2,1),1),'ubit1');
        i=i+3;
            if p==count
                break;
            end
        p=p+1;
    elseif(bitand(wa(i,1),240)==192)&&(p<=count)
        fwrite(getfid,bitget(wa(i+1,1),1),'ubit1');
         i=i+2;
            if p==count
                break;
            end
        p=p+1;
      elseif(bitand(wa(i,1),240)==208)&&(p<=count-3)
          fwrite(getfid,bitget(wa(i+1,1),1),'ubit1');
          fwrite(getfid,bitget(wa(i+1,1),2),'ubit1');
          fwrite(getfid,bitget(wa(i+1,1),3),'ubit1');
          fwrite(getfid,bitget(wa(i+1,1),4),'ubit1');
          i=i+2;
          if p==count
               break;
          end
          p=p+4;
    end
end
fclose(getfid);