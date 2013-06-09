Date
====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

値が有効な日付であることを検証します。つまり、\ ``DateTime`` オブジェクトであるか、有効なYYYY-MM-DDフォーマットに従う文字列または文字列にキャスト可能なオブジェクトであることを検証します。

+----------------+--------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`        |
+----------------+--------------------------------------------------------------------+
| オプション     | - `message`_                                                       |
+----------------+--------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Date`          |
+----------------+--------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\DateValidator` |
+----------------+--------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                birthday:
                    - Date: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Date()
             */
             protected $birthday;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="birthday">
                <constraint name="Date" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('birthday', new Assert\Date());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid date``

基になるデータが有効な日付でない場合、このメッセージが表示されます。

.. 2011/12/03 yanchi dfe0182eac4f39cea0fcacfe20ba5f26a8bd5fc7
.. 2013/06/07 hidenorigoto 9cabf0df7098a47e28515000b90f9c7446c17d6e
