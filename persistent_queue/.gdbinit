set debuginfod enabled off
b main.(*RTQueue).checkRecopy
b main.(*RTQueue).checkNormal
#b main.(*RTQueue).rtOps
b main.(*RTQueue).pop
