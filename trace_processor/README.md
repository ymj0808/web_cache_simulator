->make
->./trace_processed in_path, ref_path, out_path, rnode_num, vnode_num, from_node, to_node, ratio

in_path    : 需要处理的trace
ref_path   : 用来提供request id的trace（通过修改request id，让该请求落到其他vnode上）
out_path   : 处理好的trace地址名称
rnode_num  : 实节点数
vnode_num  : 每个实节点的虚节点数
from_node  :  
to_node    : 将落在 from_node中 vnode*ratio 个虚节点上的 request的id 修改为落在to_node上
ratio      : 

Example: ./trace_processor wikiaa wikiad wikiaa_processed 4 40 0 1 0.5
wikiaa中实节点0有40个虚节点，将落在其中的20（40*0.5）个虚节点的request的id改为落在实节点1上，wikiad用来提供修改所以需要的request id
