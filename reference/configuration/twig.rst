.. note::

    * 対象バージョン：2.3 (2.1以降)
    * 翻訳更新日：2013/11/23

.. index::
   pair: Twig; Configuration reference

TwigBundle 設定 ("twig")
=================================

.. configuration-block::

    .. code-block:: yaml

        twig:
            exception_controller:  twig.controller.exception:showAction
            form:
                resources:

                    # 初期値:
                    - form_div_layout.html.twig

                    # 設定例:
                    - MyBundle::form.html.twig
            globals:

                # 設定例:
                foo:                 "@bar"
                pi:                  3.14

                # オプション設定例 簡単な方法は下記です
                some_variable_name:
                    # サービスのidは値である必要があります
                    id:                   ~
                    # サービスを設定するか空欄にします
                    # set to service or leave blank
                    type:                 ~
                    value:                ~
            autoescape:                ~

            # Symfony 2.3. で追加されました。
            # 詳細 http://twig.sensiolabs.org/doc/recipes.html#using-the-template-name-to-set-the-default-escaping-strategy
            autoescape_service:        ~ # 設定例: @my_service
            autoescape_service_method: ~ #  autoescape_service オプションとの併用例 
            base_template_class:       ~ # 設定例: Twig_Template
            cache:                     "%kernel.cache_dir%/twig"
            charset:                   "%kernel.charset%"
            debug:                     "%kernel.debug%"
            strict_variables:          ~
            auto_reload:               ~
            optimizations:             ~

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:twig="http://symfony.com/schema/dic/twig"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd
                                http://symfony.com/schema/dic/twig http://symfony.com/schema/dic/doctrine/twig-1.0.xsd">

            <twig:config auto-reload="%kernel.debug%" autoescape="true" base-template-class="Twig_Template" cache="%kernel.cache_dir%/twig" charset="%kernel.charset%" debug="%kernel.debug%" strict-variables="false">
                <twig:form>
                    <twig:resource>MyBundle::form.html.twig</twig:resource>
                </twig:form>
                <twig:global key="foo" id="bar" type="service" />
                <twig:global key="pi">3.14</twig:global>
            </twig:config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('twig', array(
            'form' => array(
                'resources' => array(
                    'MyBundle::form.html.twig',
                )
             ),
             'globals' => array(
                 'foo' => '@bar',
                 'pi'  => 3.14,
             ),
             'auto_reload'         => '%kernel.debug%',
             'autoescape'          => true,
             'base_template_class' => 'Twig_Template',
             'cache'               => '%kernel.cache_dir%/twig',
             'charset'             => '%kernel.charset%',
             'debug'               => '%kernel.debug%',
             'strict_variables'    => false,
        ));

Configuration
-------------

.. _config-twig-exception-controller:

exception_controller
....................

**type**: ``string`` **default**: ``twig.controller.exception:showAction``

このコントローラー(exception_controller)はアプリケーション内で発生した例外を処理します。
初期設定の例外コントローラー(:class:`Symfony\\Bundle\\TwigBundle\\Controller\\ExceptionController`)は
受け取ったエラーによって、適用するテンプレートを選択します。
このオプションは上級者向けの設定です。(エラーページのカスタマイズ方法 :doc:`/cookbook/controller/error_pages`)を参照してください。
例外時の挙動の処理を行う際には、 ``kernel.exception`` イベントをキャッチする必要があります。(kernel-event-listner :ref:`dic-tags-kernel-event-listener`)を参照してください。

.. 2013/11/23 ytone d11327b2c28ebb71b9cdc1b5cf5879183905b3ad
