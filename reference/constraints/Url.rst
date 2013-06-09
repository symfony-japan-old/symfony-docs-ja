Url
===

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

検証対象の値が有効な URL 文字列であることを検証します。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `message`_                                                        |
|                | - `protocols`_                                                      |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Url`            |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\UrlValidator`   |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                bioUrl:
                    - Url:

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Url()
             */
             protected $bioUrl;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="bioUrl">
                <constraint name="Url" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfomy\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
  
        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('bioUrl', new Assert\Url());
            }
        }
  
オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid URL``

URL が無効な場合に、このメッセージが表示されます。

protocols
~~~~~~~~~

**型**: ``array`` **デフォルト**: ``array('http', 'https')``

有効とみなされるプロトコルのリストを指定します。たとえば、\ ``ftp://`` で始まる URL も有効であるとみなす場合、\ ``protocols`` の配列として ``http``\ 、\ ``https`` と ``ftp`` を指定します。

.. 2013/06/09 hidenorigoto aad266861b7e44e188914b78b9de718b29e78dd7
