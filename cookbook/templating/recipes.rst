テンプレートレシピ集
====================

.. 翻訳を更新するまで以下を表示
.. caution::

    このドキュメントの内容は古い内容です。最新の内容は公式の英語ドキュメントをご確認ください。

.. _twig_extension_tag:

Twigエクステンションを有効にする
--------------------------------

Twigエクステンションを有効にするには、設定ファイルにて ``twig.extension`` から始まるタグを用いて追加します。

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.エクステンション名:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.エクステンション名" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.エクステンション名', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

.. _templating_engine_tag:

カスタムなテンプレートエンジンを利用する
----------------------------------------

独自のテンプレートエンジンを利用するには、設定ファイルにて ``templating.engine`` から始まるタグを用いて追加します。

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.engine.テンプレートエンジン名:
                class: Fully\Qualified\Engine\Class\Name
                tags:
                    - { name: templating.engine }

    .. code-block:: xml

        <service id="templating.engine.テンプレートエンジン名" class="Fully\Qualified\Engine\Class\Name">
            <tag name="templating.engine" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.engine.テンプレートエンジン名', 'Fully\Qualified\Engine\Class\Name')
            ->addTag('templating.engine')
        ;

.. _templating_helper_tag:

PHPテンプレートヘルパーを作成する
---------------------------------

独自のテンプレートヘルパーを有効にするには、設定ファイルにて ``templating.helper`` から始まるタグを使用します。また ``alias`` 属性にエイリアス名を指定する必要があります。このエイリアス名が、テンプレートから使う場合の名前となります。

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.ヘルパー名:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.ヘルパー名" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.ヘルパー名', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;
