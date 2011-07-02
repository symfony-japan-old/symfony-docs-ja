.. 2011/07/01 jptomo 17c2f3eab78d60b89ba1

.. index::
   single: Configuration Reference; WebProfiler

WebProfilerBundle 設定
===============================

すべての設定値の説明および初期値
--------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # true に設定すると、Web デバッグツールバーに補助項目を表示します。
            verbose:             true

            # true に設定すると、Web デバッグツールバーを画面下部に表示します。
            toolbar:             false

            # true に設定すると、リダイレクトする前に集計されたデータを表示する機会を得ます。
            intercept_redirects: false