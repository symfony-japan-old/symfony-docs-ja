.. 2011/07/01 jptomo 4a091481ce92e14c7e1f8cb53919c091652bc7a8

.. index::
   single: Configuration Reference; WebProfiler

WebProfilerBundle 設定
======================

すべての設定値の説明および初期値
--------------------------------

.. configuration-block::

    .. code-block:: yaml

        web_profiler:
            
            # true に設定すると、Web デバッグツールバーに補助項目を表示します。
            verbose:             true

            # true に設定すると、Web デバッグツールバーを画面下部に表示します。
            toolbar:             false

            # true に設定すると、リダイレクトする前に集計されたデータを表示する機会を得ます。
            intercept_redirects: false
