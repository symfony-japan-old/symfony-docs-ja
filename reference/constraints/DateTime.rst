.. 2011/12/03 yanchi dfe0182eac4f39cea0fcacfe20ba5f26a8bd5fc7

DateTime
========

値が有効な"datetime"であることを検証します、``DateTime`` オブジェクトまたは文字列（または文字列にキャストできるオブジェクト）それは有効なYYYY-MM-DD HH:MM:SSフォーマットに続くいずれかであることを意味します。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`property or method<validation-property-target>`                  |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\DateTime`          |
+----------------+------------------------------------------------------------------------+
| バリデータ     | :class:`Symfony\\Component\\Validator\\Constraints\\DateTimeValidator` |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\BlobBundle\Entity\Author:
            properties:
                createdAt:
                    - DateTime: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\DateTime()
             */
             protected $createdAt;
        }

オプション
----------

メッセージ
~~~~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value is not a valid datetime``

基になるデータが有効なdatetimeでない場合、このメッセージが表示されます。
