.. 2011/07/03 jptomo 8ac37d1c76f2d6ad73fd1d24b73ee159542c719d

.. index::
   pair: Assetic; Configuration Reference

AsseticBundle 設定
=====================================

すべての設定値の説明および初期値
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        assetic:
            debug:                true
            use_controller:       true
            read_from:            %kernel.root_dir%/../web
            write_to:             %assetic.read_from%
            java:                 /usr/bin/java
            node:                 /usr/bin/node
            sass:                 /usr/bin/sass
            bundles:

                # 初期値 (全登録済みバンドル):
                - FrameworkBundle
                - SecurityBundle
                - TwigBundle
                - MonologBundle
                - SwiftmailerBundle
                - DoctrineBundle
                - AsseticBundle
                - ...

            assets:

                # プロトタイプ
                name:
                    inputs:               []
                    filters:              []
                    options:

                        # プロトタイプ
                        name:                 []
            filters:

                # プロトタイプ
                name:                 []
            twig:
                functions:

                    # プロトタイプ
                    name:                 []
