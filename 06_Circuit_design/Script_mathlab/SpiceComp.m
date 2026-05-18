function StrOut=NormaliseComp(ValIn)
    suffix = {'a' 'f' 'p' 'n' 'u' 'm' '' 'k' 'Meg' 'G' 'T' 'E'};
    
    Index = floor(log10(ValIn)/3)+7;
    %suffix{Index}
    ValExpSci = floor(log10(ValIn)/3)*3;
    ValBase = ValIn./(10.^ValExpSci);
    StrOut = sprintf('%.2f%s', ValBase, suffix{Index});

end