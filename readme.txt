このディレクトリがPC版に改良したものです．

─── ファイル構成 ───
bdnn_module/
   ├ decomposition.py ─ 分解圧縮モジュール
   ├ exhaustive_decomposer.py ─ exhaustive法によるベクトル分解モジュール(max_iter等を変更可)
   └ extract_weight.py ─ 重みの種類判別展開モジュール
utils/
   ├ parse_config.py ─ モデル定義補助ファイル
   └ utils.py ─ ユーティリティファイル
model/
   ├ coco ─ BDETR
   └ imagenet ─ BDCNN(deit)
weights/
   └ detr ─ detrの重み
      └ binary(7565, ..., 9585_2) ─ 7565(75=max_iter, 65=init_iter)
          └ B(2, ..., 8, 10) ─ 基底数2~8,10の分解済み重み

─── 使用方法 ───
python weight_decomposition.pyを開き、分解したい重みの基底数を指定して実行します
デフォルトでは基底数6,8の重み分解が実行され、分解後の重みはweights/binary2に保存されます


─── 使用例 ───
<分解> ※分解の際は全てnn.linear，nn.conv2d
python3.10 decompositionV2.py --model detr --weights ./weights/detr/detr-r50-e632da11.pth

<推論> ※上：detr，下：deit，python
python3.10 mymain.py --batch_size 2 --no_aux_loss --eval --resume https://dl.fbaipublicfiles.com/detr/detr-r50-e632da11.pth --coco_path coco --weights ./weights/detr/detr-r50-e632da11.pth

python3.10 binary_myeval_i9U.py --model deit_small --weight ./weights/deit_small/deit_small_patch16_224-cd65a155.pth


─── 分解時のターミナル表示例 ───
layer: 現在分解対象の層  param: 層の重みサイズ  decomposed: 分解後の重みサイズ
compress: 圧縮率  restore: 復元率  time: 処理時間
8bit: 8bit量子化をかけた際の推測値
確認用として重みのshapeも表示しています
-------- decomposition basis: 6 --------
     layer               param      decomposed    8bit    compress      restore     time
  module_list.0.conv_0	0.0016[MB]    0.0010[MB]   0.0004[MB]   36.8056[%]   95.3247[%]   1.8591[sec]   [16  3  3  3]
  module_list.2.conv_2	0.0176[MB]    0.0048[MB]   0.0044[MB]   72.9167[%]   91.5133[%]   3.5208[sec]   [32 16  3  3]
  module_list.4.conv_4	0.0703[MB]    0.0161[MB]   0.0176[MB]   77.0833[%]   91.1633[%]   6.1140[sec]   [64 32  3  3]
                 :                      :             :            :            :           :                :
                 :                      :             :            :            :           :                :



─── 基底数について ───
重みの基底数を大きくすると近似精度が向上しますが、そのぶん重みのファイルサイズが大きくなります
逆に小さくすると分解後の重みのファイルサイズがより小さくなりますが、近似精度が低下します
＊重み分解のイメージは重み分解方法.pdfを参照


─── 適用方法 ───
畳み込み層に分解した重みを適用する例：nn.Conv2d(backbone.num_channels, hidden_dim, kernel_size=1) -> bcnn.Conv2d_binary(backbone.num_channels, hidden_dim, kernel_size=1, quantize_bits=8, basis=8, d_mode='exh')
全結合層に分解した重みを適用する例：nn.Linear(hidden_dim, num_classes + 1) -> bnn.Linear(hidden_dim, num_classes + 1, quantize_bits=8, basis=8, d_mode='exh')



