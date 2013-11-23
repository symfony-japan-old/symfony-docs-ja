.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/32


.. index::
   single: Configuration reference; WebProfiler

WebProfilerBundle 設定 ("web_profiler")
================================================

すべての初期設定
--------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:

            # 非推奨, この設定は現在有効ではありません。設定から削除しても問題ありません。
            verbose:              true

            # true に設定すると、webデバッグツールバーが表示されます。positionでページの表示位置を設定できます。
            toolbar:              false
            position:             bottom

            # true に設定すると、リダイレクトの時にweb profilerを確認することができます。
            intercept_redirects: false

    .. code-block:: xml

        <web-profiler:config
            toolbar="false"
            verbose="true"
            intercept_redirects="false"
        />

.. 2013/11/23 ytone 74b92f5a6e6085b8718c727185d833d841061cf3


