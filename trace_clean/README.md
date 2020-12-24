两个功能：1)统一同一request的size（统一为第一次出现的size）<br>
          2)清洗请求次数低于n的请求（n是可以输入的参数）<br>
<br>
->make<br>
->./trace_clean in_path out_path [threshold]<br>
<br>
in_path    : 需要处理的trace<br> 
out_path   : 处理好的trace地址名称<br>
threshold  : 请求次数 <= threshold 的请求会被删去（若为0，只统一size）
<br>
Example: ./trace_clean in_path out_path 1
将请求次数小于2的请求都删除
