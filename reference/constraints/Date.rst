.. 2011/12/03 yanchi dfe0182eac4f39cea0fcacfe20ba5f26a8bd5fc7

Date
====

値が有効な日付であることを検証します、 ``DateTime`` オブジェクトまたは文字列（または文字列にキャストできるオブジェクト）それは有効なYYYY-MM-DDフォーマットに続くいずれかであることを意味します。

+----------------+--------------------------------------------------------------------+
| 適用先         | :ref:`property or method<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| オプション     | - `message`_                                                       |
+----------------+--------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Date`          |
+----------------+--------------------------------------------------------------------+
| バリデータ     | :class:`Symfony\\Component\\Validator\\Constraints\\DateValidator` |
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
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Date()
             */
             protected $birthday;
        }

オプション
----------

メッセージ
~~~~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value is not a valid date``

基になるデータが有効な日付でない場合、このメッセージが表示されます。
