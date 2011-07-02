.. index::
   single: Configuration Reference; WebProfiler

WebProfilerBundle Configuration
===============================

Full Default Configuration
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