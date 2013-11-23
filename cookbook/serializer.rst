.. index::
   single: Serializer

.. note::

    * 対象バージョン : 2.3
    * 翻訳更新日 : 2013/11/23

Serializerの使い方
=========================

オブジェクトと他のフォーマット（たとえば JSON や XML）の間のシリアライズとデシリアライズはとても複雑なトピックです。
Symfony には :doc:`Serializer コンポーネント</components/serializer>` があり
、便利なツールが使えるようになっています。


読み進める前に、 :doc:`Serializer コンポーネントのドキュメント</components/serializer>` を読んで、 serializer、normalizer、encoderに慣れてください。
`JMSSerializerBundle`_ は Symfony のコアのシリアライザを拡張して便利な機能を追加しているので、こちらも見てみてください。

Serializerを有効化する
-------------------------

.. versionadded:: 2.3
    シリアライザは以前から Symfony に含まれていましたが、2.3 より前のバージョンでは ``serializer`` サービスをわざわざ定義しなければなりませんでした。

``serializer`` サービスはデフォルトでは使用できません。シリアライザを使うためには、設定ファイルにより有効化してください。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            serializer:
                enabled: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:serializer enabled="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'serializer' => array(
                'enabled' => true
            ),
        ));

Normalizer や Encoders を追加する
----------------------------------

有効化すると、 ``serializer`` サービスはコンテナから取得できるようになり、2つの :ref:`encoder<component-serializer-encoders>` （ :class:`Symfony\\Component\\Serializer\\Encoder\\JsonEncoder` と :class:`Symfony\\Component\\Serializer\\Encoder\\XmlEncoder` ）が利用できます。
しかし、 :ref:`normalizer<component-serializer-normalizers>` はありません。 独自の normarilizer を追加する必要があります。

normalizer や encoder は、 :ref:`serializer.normalizer<reference-dic-tags-serializer-normalizer>` タグと :ref:`serializer.encoder<reference-dic-tags-serializer-encoder>` タグをつけることにより追加することができます。
マッチする順番を決められるように、タグに ``priority`` を設定することもできます。

下記は :class:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer` サービスをシリアライザに追加する方法のサンプルです。

.. configuration-block:: 

    .. code-block:: yaml

       # app/config/config.yml
       services:
          get_set_method_normalizer:
             class: Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer
             tags:
                - { name: serializer.normalizer }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="get_set_method_normalizer" class="Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer">
                <tag name="serializer.normalizer" />
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $definition = new Definition(
            'Symfony\Component\Serializer\Normalizer\GetSetMethodNormalizer'
        ));
        $definition->addTag('serializer.normalizer');
        $container->setDefinition('get_set_method_normalizer', $definition);

.. note::

    :class:`Symfony\\Component\\Serializer\\Normalizer\\GetSetMethodNormalizer` はもとから壊れています。循環参照のあるオブジェクトを処理させると、ゲッターメソッドを呼び出した際に無限ループが発生します。
    実際のユースケースにしたがって、独自の normalizer を作って利用してください。

.. _JMSSerializerBundle: http://jmsyst.com/bundles/JMSSerializerBundle

.. 2013/11/23 77web a9c8ed63303e05552adbc398eeb4bfd8ef3ff255
