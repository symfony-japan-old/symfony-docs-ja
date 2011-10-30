特定の拡張子に Assetic フィルターを適用するには？
=================================================

Assetic フィルターは、個々のファイル、ファイルを集めたグループ、そして、この記事で説明するように特定の拡張子を持つファイル群、に対して適用することができます。例として Assetic の CoffeScript フィルターを使って JavaScript にコンパイルするケースを見ていきましょう。

メインの コンフィギュレーション に cofee と node へのパスを指定します。デフォルトでは、それぞれ ``/usr/bin/coffee`` と ``/usr/bin/node`` になります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin' => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                ),
            ),
        ));

１つのファイルをフィルターする
------------------------------

上記の指定で、テンプレート上で １つの CoffeeScript ファイルを JavaScript に出力することができるようになりました。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
            filter='coffee'
        %}
        <script src="{{ asset_url }} type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee'),
            array('coffee')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

CoffeeScript ファイルをコンパイルして JavaScript に変更するのに必要なことは、これだけです。

複数のファイルをフィルターする
------------------------------

複数の CoffeeScript ファイルをコンパイルして、１つのファイルとしてまとめて出力することもできます。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
            filter='coffee'
        %}
        <script src="{{ asset_url }} type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee',
                  '@AcmeFooBundle/Resources/public/js/another.coffee'),
            array('coffee')) as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

これで２つのファイルをコンパイルして １つの JavaScript ファイルとして出力することができました。

ファイル拡張子に基づいてフィルターする
--------------------------------------

Assetic を使用する大きなアドバンテージは、たくさんのアセットファイルを固めることで HTTP リクエストを減らすことです。 *全て* のJavaScript ファイルや CoffeeScript ファイルを一緒に結合させて、最終的に１つの JavaScript ファイルとして出力できたら、 HTTP リクエストを減らすことができます。しかし、その JavaScript ファイルを上記の結合するファイルのリストに加えるだけでは、 CoffeeScript のコンパイルとバッティングしてしまいます。

この問題を避けるには、コンフィギュレーションの ``apply_to`` オプションを使用して、特定の拡張子に常に適用されるフィルターを指定してください。下記の設定では、全ての ``.coffee`` 拡張子を持つファイルを Coffee フィルターに適用させています。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        assetic:
            filters:
                coffee:
                    bin: /usr/bin/coffee
                    node: /usr/bin/node
                    apply_to: "\.coffee$"

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <assetic:config>
            <assetic:filter
                name="coffee"
                bin="/usr/bin/coffee"
                node="/usr/bin/node"
                apply_to="\.coffee$" />
        </assetic:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('assetic', array(
            'filters' => array(
                'coffee' => array(
                    'bin' => '/usr/bin/coffee',
                    'node' => '/usr/bin/node',
                    'apply_to' => '\.coffee$',
                ),
            ),
        ));

これでテンプレート内で ``coffree`` フィルターを指定する必要がなくなりました。また、 ``.coffee`` 拡張子のファイルのみが CoffeeScript フィルターを通過するので、同じように JavaScript ファイルもリストに加えて、１つの JavaScript ファイルとして結合することができるようになりました。

.. configuration-block::

    .. code-block:: html+jinja

        {% javascripts '@AcmeFooBundle/Resources/public/js/example.coffee'
                       '@AcmeFooBundle/Resources/public/js/another.coffee'
                       '@AcmeFooBundle/Resources/public/js/regular.js'
        %}
        <script src="{{ asset_url }} type="text/javascript"></script>
        {% endjavascripts %}

    .. code-block:: html+php

        <?php foreach ($view['assetic']->javascripts(
            array('@AcmeFooBundle/Resources/public/js/example.coffee',
                  '@AcmeFooBundle/Resources/public/js/another.coffee',
                  '@AcmeFooBundle/Resources/public/js/regular.js'),
            as $url): ?>
        <script src="<?php echo $view->escape($url) ?>" type="text/javascript"></script>
        <?php endforeach; ?>

.. 2011/10/25 ganchiku 74788cbbdc762ef7c59ca6c512fc0e1aea503b9d

