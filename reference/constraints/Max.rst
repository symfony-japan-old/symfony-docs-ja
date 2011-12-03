.. 2011/12/03 yanchi dfe0182eac4f39cea0fcacfe20ba5f26a8bd5fc7

Max
===

与えられた数字が最大数*以下*であることを検証します。

+----------------+--------------------------------------------------------------------+
| 適用先         | :ref:`property or method<validation-property-target>`              |
+----------------+--------------------------------------------------------------------+
| オプション     | - `limit`_                                                         |
|                | - `message`_                                                       |
|                | - `invalidMessage`_                                                |
+----------------+--------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Max`           |
+----------------+--------------------------------------------------------------------+
| バリデータ     | :class:`Symfony\\Component\\Validator\\Constraints\\MaxValidator`  |
+----------------+--------------------------------------------------------------------+

基本的な使い方
--------------

クラスの"年齢"フィールドが"50"を超えていないことを確認するには、次の行を追加します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/EventBundle/Resources/config/validation.yml
        Acme\EventBundle\Entity\Participant:
            properties:
                age:
                    - Max: { limit: 50, message: You must be 50 or under to enter. }

    .. code-block:: php-annotations

        // src/Acme/EventBundle/Entity/Participant.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Participant
        {
            /**
             * @Assert\Max(limit = 50, message = "You must be 50 or under to enter.")
             */
             protected $age;
        }

オプション
----------

リミット
~~~~~~~~

**タイプ**: ``integer`` [:ref:`default option<validation-default-option>`]

この必須オプションは、"最大"の値です。与えられた値がこの最大値よりも**大きい**場合、検証は失敗します。

メッセージ
~~~~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value should be {{ limit }} or less``

基になる値が`limit`_オプションより大きい場合、そのメッセージが表示されます。

invalidMessage
~~~~~~~~~~~~~~

**タイプ**: ``string`` **デフォルト**: ``This value should be a valid number``

基になる値が数値（`IS_NUMERIC`_ PHPの関数によって）でない場合、表示されるメッセージ。

.. _`is_numeric`: http://www.php.net/manual/en/function.is-numeric.php
